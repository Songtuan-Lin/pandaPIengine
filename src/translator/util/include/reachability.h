#ifndef _reachability_inc_h_
#define _reachability_inc_h_

#include "Model.h"
#include "assert.h"

class TaskReachability {
    private:
        Model *htn;
        vector<vector<int>> count;
        void dfs(int c, vector<bool> &visited);

    public:
        TaskReachability(Model *htn);
        int getActionNum(int c, int a) {
            assert(this->htn->isPrimitive[a] && !this->htn->isPrimitive[c]);
            int offset = this->htn->numActions;
            return this->count[c - offset][a];
        }
};

#endif