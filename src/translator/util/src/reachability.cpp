#include "reachability.h"
#include "assert.h"

TaskReachability::TaskReachability(Model *htn) {
    this->htn = htn;
    int offset = htn->numActions;
    this->count.resize(htn->numTasks - offset);
    vector<bool> visited;
    visited.assign(htn->numTasks - offset, false);
    for (int c = htn->numActions; c < htn->numTasks; c++) {
        int index = c - offset;
        this->dfs(c, visited);
    }
}

void TaskReachability::dfs(int c, vector<bool> &visited) {
    assert(!htn->isPrimitive[c]);
    int offset = htn->numActions;
    if (visited[c]) return;
    visited[c] = true;
    vector<int> result;
    result.assign(htn->numActions, -1);
    for (int m = 0; m < htn->numMethodsForTask[c]; m++) {
        vector<int> local;
        local.assign(htn->numActions, 0);
        for (int i = 0; i < htn->numSubTasks[m]; i++) {
            int t = htn->subTasks[m][i];
            if (htn->isPrimitive[t]) {
                local[t] = 1;
            } else {
                this->dfs(t, visited);
                for (int a = 0; a < htn->numActions; a++)
                    local[a] += this->count[t][a];
            }
        }
        for (int a = 0; a < htn->numActions; a++) {
            if (result[a] == -1) {
                result[a] = local[a];
            } else {result[a] = min(local[a], result[a]);}
        }
    }
    this->count[c] = result;
}