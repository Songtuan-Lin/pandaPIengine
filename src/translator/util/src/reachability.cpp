#include "reachability.h"
#include "assert.h"

TaskReachability::TaskReachability(Model *htn) {
    int offset = htn->numActions;
    this->count.resize(htn->numTasks - offset);
    vector<bool> visited;
    visited.assign(htn->numTasks - offset, false);
    for (int c = htn->numActions; c < htn->numTasks; c++) {
        int index = c - offset;
        this->count[index] = this->dfs(c, visited);
    }
}