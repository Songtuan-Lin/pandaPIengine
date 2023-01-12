#include "optimize.h"

OptimizeHTN::OptimizeHTN(Model *htn, vector<int> plan) {
    string prefix = "[Optimizing the Model] ";
    TDG *tdg = new TDG(htn);
    cout << prefix << "Analyzing the TDG" << endl;
    this->invalidMethods.assign(htn->numMethods, false);
    this->invalidTasks.assign(htn->numTasks, false);
    for (int i = 0; i < htn->numActions; i++)
        this->invalidTasks[i] = true;
    for (int a : plan)
        this->invalidTasks[a] = false;
    int count = 0;
    bool saturated = false;
    while (!saturated) {
        saturated = true;
        MethodDFS *mDFS = new MethodDFS(htn, this->invalidTasks);
        for (int m = 0; m < htn->numMethods; m++) {
            if (mDFS->isInvalidMethod(m) && (!this->invalidMethods[m])) {
                this->invalidMethods[m] = true;
                saturated = false;
                tdg->maskMethod(m);
                count++;
            }
        }
        int initTask = htn->initialTask;
        int v = tdg->vT(initTask);
        tdg->calcReachability(v);
        for (int m = 0; m < htn->numMethods; m++) {
            int w = tdg->vM(m);
            if ((!tdg->reachable(w)) && (!this->invalidMethods[m])) {
                this->invalidMethods[m] = true;
                tdg->maskMethod(m);
                count++;
                saturated = false;
            }
        }
        for (int t = 0; t < htn->numTasks; t++) {
            bool allDisabled = true;
            if (htn->isPrimitive[t]) continue;
            for (int i = 0; i < htn->numMethodsForTask[t]; i++) {
                int m = htn->taskToMethods[t][i];
                if (!this->invalidMethods[m]) {
                    allDisabled = false;
                    break;
                }
            }
            if (allDisabled) this->invalidTasks[t] = true;
        }
    }
    cout << prefix << "Number of redundant methods: " << to_string(count) << endl;
}