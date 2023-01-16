#ifndef _accumulation_inc_h_
#define _accumulation_inc_h_

#include "Model.h"

class ActionAccumulation {
    private:
        vector<vector<int>> accumulation;
    
    public:
        ActionAccumulation(Model *htn, vector<int> plan) {
            accumulation.resize(plan.size());
            for (int i = 0; i < plan.size(); i++) {
                if (i == 0) {
                    accumulation[i].assign(htn->numActions, 0);
                } else {accumulation[i] = accumulation[i - 1];}
                accumulation[i][plan[i]] += 1;
            }
        }
        int getNumAccumulation(int pos, int action) {return accumulation[pos][action];}
};

#endif