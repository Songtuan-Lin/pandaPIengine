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