//
// Created by u6162630 on 4/3/23.
//

#include "constraints.h"
ConstraintsOnStates::ConstraintsOnStates(
        void *solver,
        Model *htn,
        StateVariables *vars,
        PlanExecution *execution) {
    int length = execution->getStateSeqLen();
    int numProps = htn->numStateBits;
    for (int pos = 0; pos < length; pos++) {
        for (int prop = 0; prop < numProps; prop++) {
            int var = vars->get(pos, prop);
            if (execution->isPropTrue(pos, prop)) {
                assertYes(solver, var);
            } else {assertNot(solver, var);}
        }
    }
}

ConstraintsOnMapping::ConstraintsOnMapping(
        void *solver,
        sat_capsule &capsule,
        vector<int> &plan,
        PlanToSOGVars *mapping,
        SOG *sog) {
    vector<vector<int>> invMapping(sog->numberOfVertices);
    for (int pos = 0; pos < plan.size(); pos++) {
        // get the variable indicating whether
        // the plan step is matched to a vertex
        int matchedVar = mapping->getMatchedVar(pos);
        // every plan step must match to some vertex
        assertYes(solver, matchedVar);
        vector<int> possibleMappings;
        for (int v = 0; v < sog->numberOfVertices; v++){
            // get the variable representing the mapping
            // between a plan step and a vertex
            int posToVertex = mapping->getPosToVertexVar(pos, v);
            int taskVar = mapping->getTaskVar(pos, v);
            if (taskVar == -1) {
                assertNot(solver, posToVertex);
            } else {
                implies(solver, posToVertex, taskVar);
                possibleMappings.push_back(posToVertex);
            }
            int vertexToPos = mapping->getVertexToPosVar(v, pos);
            vector<int> artiPrims = mapping->getArtificialPrims(v);
            if (artiPrims.size() == 0) {
                assertNot(solver, vertexToPos);
            } else {
                impliesOr(solver, vertexToPos, artiPrims);
            }
            // TODO: mapping a vertex to a position
            int forbiddenVar = mapping->getForbiddenVar(pos, v);
            impliesNot(solver, forbiddenVar, posToVertex);
            for (const int successor : sog->adj[v]) {
                if (pos > 0) {
                    int forbidPrevNext = mapping->getForbiddenVar(
                            pos - 1,
                            successor);
                    implies(solver,
                            forbiddenVar,
                            forbidPrevNext);
                }
                int forbiddenNext = mapping->getForbiddenVar(
                        pos, successor);
                implies(solver, forbiddenVar, forbiddenNext);
            }
            if (pos > 0) {
                int forbiddenPrev = mapping->getForbiddenVar(
                        pos - 1, v);
                implies(solver, forbiddenVar, forbiddenPrev);
            }
        }
        // every plan step can be mapped to at most one
        // vertex that has the respective action
        atMostOne(solver, capsule, possibleMappings);
        // if the plan step is matched, then it must be mapped
        // to some vertex that has the respective action
        impliesOr(solver, matchedVar, possibleMappings);
    }
    for (int v = 0; v < sog->numberOfVertices; v++) {
        int activatedVar = mapping->getActivatedVar(v);
        // if the vertex is activated, it must be mapped
        // to some plan step
        assert(invMapping[v].size() == plan.size());
        impliesOr(solver, activatedVar, invMapping[v]);
        vector<int> primVars;
        PDT *pdt = sog->leafOfNode[v];
        for (const int t : pdt->possiblePrimitives) {
            int primVar = pdt->primitiveVariable[t];
            primVars.push_back(primVar);
        }
        // if the vertex is not activated, then all actions
        // in this vertex cannot be activated
        notImpliesAllNot(solver, activatedVar, primVars);
    }
}