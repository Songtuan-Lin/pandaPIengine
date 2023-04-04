//
// Created by u6162630 on 4/3/23.
//

#ifndef PANDAPIENGINE_CONSTRAINTS_H
#define PANDAPIENGINE_CONSTRAINTS_H
#include "variables.h"
#include "sog.h"
#include "execution.h"
#include "sat_encoder.h"

class ConstraintsOnMapping {
public:
    ConstraintsOnMapping(
            void *solver,
            sat_capsule &capsule,
            vector<int> &plan,
            PlanToSOGVars *mapping,
            SOG *sog);
};

class ConstraintsOnStates {
public:
    ConstraintsOnStates(
            void *solver,
            Model *htn,
            StateVariables *vars,
            PlanExecution *execution);
};

class ConstraintsOnPrecs {
public:
    ConstraintsOnPrecs(
            void *solver,
            Model *htn,
            int action,
            int pos,
            int premise,
            StateVariables *vars) {
        int numPrecs = htn->numPrecs[action];
        for (int i = 0; i < numPrecs; i++) {
            int prop = htn->precLists[action][i];
            int var = vars->get(pos, prop);
            implies(solver, premise, var);
        }
    }
};
#endif //PANDAPIENGINE_CONSTRAINTS_H
