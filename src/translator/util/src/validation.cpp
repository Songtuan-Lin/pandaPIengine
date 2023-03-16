#include "validation.h"

bool SlotValidation::validate(int m, int b, int i, TaskTraversal *traversal, ActionAccumulation *accumulation) {
    // TODO: what about compound tasks decomposed into empty task network?
    if (b > 0) {
        int count = 0;
        for (int t = 0; t < b; t++) {
            int subTask = this->htn->subTasks[m][t];
            count += traversal->getNumReachable(subTask);
        }
        if (count > i) return false;
        for (int a = 0; a < this->htn->numActions; a++) {
            int numAccumulation = 0;
            if (i > 0)
                numAccumulation = accumulation->getNumAccumulation(i - 1, a);
            int accumulationBefore = 0;
            for (int j = 0; j < b; j++) {
                int taskBefore = this->htn->subTasks[m][j];
                accumulationBefore += traversal->getNumAction(taskBefore, a);
            }
            if (numAccumulation < accumulationBefore) return false;
        }
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
            int accumulationEnd = accumulation->getNumAccumulation(this->plan.size() - 1, a);
            int accumulationStart = accumulation->getNumAccumulation(i, a);
            int numAccumulation = accumulationEnd - accumulationStart;
            int accumulationBehind = 0;
            for (int j = b; j < this->htn->numSubTasks[m]; j++) {
                int taskBehind = this->htn->subTasks[m][j];
                accumulationBehind += traversal->getNumAction(taskBehind, a);
            }
            if (numAccumulation < accumulationBehind) return false;
        }
    }
    return true;
}