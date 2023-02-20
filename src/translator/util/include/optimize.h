#ifndef _optimize_htn_inc_h_
#define _optimize_htn_inc_h_

#include "Model.h"
#include "graph.h"
#include "traversal.h"

class MethodDFS {
    public:
        MethodDFS(Model *htn, vector<bool> forbidTask) {
            this->htn = htn;
            this->invalidMethods.assign(this->htn->numMethods, false);
            this->visited.assign(this->htn->numMethods, false);
            for (int m = 0; m < this->htn->numMethods; m++)
                dfs(m, forbidTask);
        }
        bool isInvalidMethod(int m) {return this->invalidMethods[m];}

    private:
        Model *htn; 
        vector<bool> invalidMethods;
        vector<bool> visited;
        void dfs(int m, vector<bool> invalidTasks) {
            if (this->visited[m]) return;
            this->visited[m] = true;
            for (int i = 0; i < this->htn->numSubTasks[m]; i++) {
                int t = this->htn->subTasks[m][i];
                if (invalidTasks[t]) {
                    this->invalidMethods[m] = true;
                    return;
                } else if (!this->htn->isPrimitive[t]) {
                    for (int j = 0; j < this->htn->numMethodsForTask[t]; j++) {
                        int om = this->htn->taskToMethods[t][j];
                        dfs(om, invalidTasks);
                        if (!this->invalidMethods[om]) return;
                    }
                    this->invalidMethods[m] = true;
                }
            }
        }
};

class OptimizeHTN {
    private:
        vector<bool> invalidMethods;
        vector<bool> invalidTasks;
    
    public:
        OptimizeHTN(Model *htn, TaskTraversal *traversal, vector<int> plan);
        bool isTaskInvalid(int t) {return this->invalidTasks[t];}
        bool isMethodInvalid(int m) {return this->invalidMethods[m];}
};
#endif