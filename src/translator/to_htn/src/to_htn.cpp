#include "to_htn.h"

HTNTranslator::HTNTranslator(string htnFile, string planFile) : Translator(htnFile, planFile) {
    TaskTraversal *traversal = new TaskTraversal(this->htn);
    // creating propositions
    MatchingProps matchingProps(this->plan.size(), this->h.getNumProps());
    for (int m = 0; m < this->htn->numMethods; m++) {
        int minReachableTasks = 0;
        for (int t = 0; t < this->htn->numSubTasks[m]; t++)
            minReachableTasks += traversal->getNumReachable(t);
        int bound = this->plan.size() - minReachableTasks;
        CounterProps localCounterProps(bound, this->h.getNumProps());
    }

}