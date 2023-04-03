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
    ConstraintsOnMapping(void *solver, PlanToSOGVars *mapping, SOG *sog);
};

class ConstraintsOnStates {
public:
    ConstraintsOnStates(void *solver, Model *htn, StateVariables *vars, PlanExecution *execution);
};
#endif //PANDAPIENGINE_CONSTRAINTS_H
