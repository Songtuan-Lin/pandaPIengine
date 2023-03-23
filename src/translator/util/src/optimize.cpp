#include "optimize.h"

OptimizeHTN::OptimizeHTN(Model *htn, TaskTraversal *traversal, vector<int> plan) {
    string prefix = "[Optimizing the Model] ";
    TDG *tdg = new TDG(htn);
    cout << prefix << "Analyzing the TDG" << endl;
    this->invalidMethods.assign(htn->numMethods, false);
    this->invalidTasks.assign(htn->numTasks, false);
    unordered_set<int> actionsInPlan(plan.begin(), plan.end());
    for (int i = 0; i < htn->numActions; i++)
        this->invalidTasks[i] = true;
    for (int a : plan)
        this->invalidTasks[a] = false;
    int count = 0;
    for (int m = 0; m < htn->numMethods; m++) {
        int totalMinReachableTasks = 0;
        for (int i = 0; i < htn->numSubTasks[m]; i++)
            totalMinReachableTasks += traversal->getNumReachable(
                    htn->subTasks[m][i]);
        if (totalMinReachableTasks > plan.size()) {
            this->invalidMethods[m] = true;
            tdg->maskMethod(m);
            count++;
        }
    }
    bool saturated = false;
    while (!saturated) {
        saturated = true;
        for (int m = 0; m < htn->numMethods; m++) {
            if (this->invalidMethods[m]) {
                continue;
            }
            assert(!this->invalidMethods[m]);
            int u = tdg->vM(m);
            vector<int>::iterator iter;
            for (iter = tdg->adjBegin(u); iter < tdg->adjEnd(u); iter++) {
                int adjTask = tdg->T(*iter);
                if (this->invalidTasks[adjTask] && (actionsInPlan.count(adjTask) == 0)) {
                    this->invalidMethods[m] = true;
                    tdg->maskMethod(m);
                    saturated = false;
                    count++;
                    break;
                }
            }
        }
        int initTask = htn->initialTask;
        int v = tdg->vT(initTask);
        tdg->calcReachability(v);
        for (int t = 0; t < htn->numTasks; t++) {
            int w = tdg->vT(t);
            if (!tdg->reachable(w) && (!this->invalidTasks[t])) {
#ifndef NDEBUG
                for (int pos = 0; pos < plan.size(); pos++) {
                    int a = plan[pos];
                    if (t == a)
                        cout << prefix << "Unreachable action in position: " << pos << endl;
                }
#endif
                this->invalidTasks[t] = true;
                tdg->maskTask(t);
                saturated = false;
            }
        }
        for (int m = 0; m < htn->numMethods; m++) {
            int w = tdg->vM(m);
            if ((!tdg->reachable(w)) && (!this->invalidMethods[m])) {
                assert(!this->invalidMethods[m]);
                this->invalidMethods[m] = true;
                tdg->maskMethod(m);
                count++;
                saturated = false;
            }
        }
        for (int t = 0; t < htn->numTasks; t++) {
            bool allDisabled = true;
            if (htn->isPrimitive[t]) {
                continue;
            }
            for (int i = 0; i < htn->numMethodsForTask[t]; i++) {
                int m = htn->taskToMethods[t][i];
                if (!this->invalidMethods[m]) {
                    allDisabled = false;
                    break;
                }
            }
            if (allDisabled && this->invalidTasks[t] == false) {
                this->invalidTasks[t] = true;
                tdg->maskTask(t);
                saturated = false;
            }
        }
    }
    cout << prefix << "Number of redundant methods: " << to_string(count) << endl;
    if (this->invalidTasks[htn->initialTask]) {
        cout << prefix << "Initial task identified as invalid" << endl;
        exit(-1);
    }
}