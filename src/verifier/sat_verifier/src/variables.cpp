#include "variables.h"

PlanToSOGVars::PlanToSOGVars(
        vector<int> &plan,
        SOG *sog,
        Model *htn,
        sat_capsule &capsule) {
    for (int pos = 0; pos < plan.size(); pos++) {
        int p = plan[pos];
        vector<int> mappingPerPos;
        mappingPerPos.assign(sog->numberOfVertices, -1);
        vector<int> forbiddenPerPos;
        forbiddenPerPos.assign(sog->numberOfVertices, -1);
        vector<int> taskPerPos;
        taskPerPos.assign(sog->numberOfVertices, -1);

        int var = capsule.new_variable();
        string name = "matched[" + to_string(pos) + "]";
        DEBUG(capsule.registerVariable(var, name));
        this->matched.push_back(var);

        for (int v = 0; v < sog->numberOfVertices; v++) {
            var = capsule.new_variable();
            name = "mapping[" + to_string(pos) + ";" + to_string(v) + "]";
            DEBUG(capsule.registerVariable(var, name));
            mappingPerPos[v] = var;

            var = capsule.new_variable();
            name = "forbidden[" + to_string(pos) + ";" + to_string(v) + "]";
            DEBUG(capsule.registerVariable(var, name));
            forbiddenPerPos[v] = var;

            PDT *pdt = sog->leafOfNode[v];
            for (size_t t = 0; t < pdt->possiblePrimitives.size(); t++) {
                int action = pdt->possiblePrimitives[t];
                if (action == p) {
                    var = pdt->primitiveVariable[t];
                    taskPerPos[v] = var;
                    break;
                }
            }
        }
        this->mapping.push_back(mappingPerPos);
        this->forbidden.push_back(forbiddenPerPos);
        this->tasks.push_back(taskPerPos);
    }
}