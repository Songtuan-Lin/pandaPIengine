#include "traversal.h"
#include "assert.h"

TaskTraversal::TaskTraversal(Model *htn) {
    this->htn = htn;
    int offset = htn->numActions;
    this->count.resize(htn->numTasks - offset);
    this->numReachableTasks.resize(htn->numTasks - offset);
    vector<bool> visited;
    visited.assign(htn->numTasks - offset, false);
    cout << "[Do DFS on tasks and methods]";
    for (int c = htn->numActions; c < htn->numTasks; c++)
        this->dfs(c, visited);
    cout << " Done!" << endl;
}

void TaskTraversal::dfs(int c, vector<bool> &visited) {
    assert(!htn->isPrimitive[c]);
    int offset = htn->numActions;
    if (visited[c - offset]) return;
    visited[c - offset] = true;
    vector<int> result;
    result.assign(htn->numActions, -1);
    int num = -1;
    for (int i = 0; i < htn->numMethodsForTask[c]; i++) {
        int m = htn->taskToMethods[c][i];
        vector<int> local;
        local.assign(htn->numActions, 0);
        int n = 0;
        for (int i = 0; i < htn->numSubTasks[m]; i++) {
            int t = htn->subTasks[m][i];
            if (htn->isPrimitive[t]) {
                local[t] = 1;
                n += 1;
            } else {
                this->dfs(t, visited);
                for (int a = 0; a < htn->numActions; a++)
                    local[a] += this->count[t - offset][a];
                n += this->numReachableTasks[t - offset];
            }
        }
        for (int a = 0; a < htn->numActions; a++) {
            if (result[a] == -1) {
                result[a] = local[a];
            } else {result[a] = min(local[a], result[a]);}
        }
        if (num == -1) {
            num = n;
        } else {num = min(num, n);}
    }
    this->count[c - offset] = result;
    this->numReachableTasks[c - offset] = num;
}