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
        vector<int> &plan,
        PlanToSOGVars *mapping,
        SOG *sog) {
    for (int pos = 0; pos < plan.size(); pos++) {
        // get the variable indicating whether
        // the plan step is matched to a vertex
        int matchedVar = mapping->getMatchedVar(pos);
        // every plan step must match to some vertex
        assertYes(solver, matchedVar);
        for (int v = 0; v < sog->numberOfVertices; v++){
            // get the variable representing the mapping
            // between a plan step and a vertex
            int mappingVar = mapping->getMappingVar(pos, v);
            if (mappingVar == -1) continue;
            int taskVar = mapping->getTaskVar(pos, v);
            assert(taskVar != -1);
            implies(solver, mappingVar, taskVar);
        }
    }
}