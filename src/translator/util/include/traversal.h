#ifndef _reachability_inc_h_
#define _reachability_inc_h_

#include "Model.h"
#include "assert.h"

class TaskTraversal {
    private:
        Model *htn;
        vector<vector<int>> count;
        vector<int> numReachableTasks;
        void dfs(int c, vector<bool> &visited);

    public:
        TaskTraversal(Model *htn);
        int getActionNum(int c, int a) {
            assert(this->htn->isPrimitive[a] && !this->htn->isPrimitive[c]);
            int offset = this->htn->numActions;
            return this->count[c - offset][a];
        }
        int getNumReachable(int t) {
            if (this->htn->isPrimitive[t]) return 1;
            int offset = this->htn->numActions;
            return this->numReachableTasks[t - offset];
        }
};

#endif