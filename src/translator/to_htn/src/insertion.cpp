#include "insertion.h"

bool ActionInsertions::validate(int m, int b, int i, TaskTraversal *traversal, ActionAccumulation *accumulation) {
    // TODO: what about compound tasks decomposed into empty task network?
    if (b > 0) {
        int count = 0;
        for (int t = 0; t < b; t++) {
            int subTask = this->htn->subTasks[m][t];
            count += traversal->getNumReachable(subTask);
        }
        if (count > i) return false;
        int prevTask = this->htn->subTasks[m][b - 1];
        for (int a = 0; a < this->htn->numActions; a++)
            if (accumulation->getNumAccumulation(i - 1, a) < traversal->getNumAction(prevTask, a)) 
                return false;
    }
    if (b < this->htn->numSubTasks[m]) {
        int count = 0;
        for (int t = b; t < this->htn->numSubTasks[m]; t++) {
            int subTask = this->htn->subTasks[m][t];
            count += traversal->getNumReachable(subTask);
        }
        if (count > (this->plan.size() - 1 - i)) return false;
        int nextTask = this->htn->subTasks[m][b];
        for (int a = 0; a < this->htn->numActions; a++) {
            int accumulationEnd = accumulation->getNumAccumulation(this->plan.size(), a);
            int accumulationStart = accumulation->getNumAccumulation(i, a);
            int numAccumulation = accumulationEnd - accumulationStart;
            if (numAccumulation < traversal->getNumAction(nextTask, a)) return false;
        }
    }
    return true;
}