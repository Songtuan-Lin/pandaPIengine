#ifndef _reachability_inc_h_
#define _reachability_inc_h_

#include "Model.h"

class TaskReachability {
    private:
        vector<vector<int>> count;
        vector<int> dfs(int c, vector<bool> &visited);

    public:
        TaskReachability(Model *htn);
};

#endif