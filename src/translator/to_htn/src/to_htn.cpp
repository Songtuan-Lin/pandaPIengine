#include "to_htn.h"

HTNTranslator::HTNTranslator(string htnFile, string planFile) : Translator(htnFile, planFile) {
    TaskTraversal *traversal = new TaskTraversal(this->htn);
    SlotValidiation *validation = new SlotValidiation(this->htn, this->plan);
    // creating propositions
    PropsForMatching propsForMatching(this->plan.size(), this->h.getNumProps());
    this->h.addProps(propsForMatching.get());
    int maxInsertions = this->plan.size() - traversal->getNumReachable(this->htn->initialTask);
    PropsForCounters propsForGlobalCounter(maxInsertions, this->h.getNumProps()); 
    this->h.addProps(propsForGlobalCounter.get());
    for (int m = 0; m < this->htn->numMethods; m++) {
        int minReachableTasks = 0;
        for (int t = 0; t < this->htn->numSubTasks[m]; t++)
            minReachableTasks += traversal->getNumReachable(t);
        int bound = this->plan.size() - minReachableTasks;
        PropsForCounters propsForLocalCounter(bound, this->h.getNumProps());
        this->h.addProps(propsForLocalCounter.get());
        for (int b = 0; b < this->htn->numSubTasks[m]; b++)
            for (int i = 0; i < this->plan.size(); i++) {
                if (!validation->isValid(m, b, i)) continue;
                Slot s(m, b, i);
                PropsForInsertions propIns(s, this->h.getNumProps());
                this->h.addProps(propIns.get());
            }
    }
    // creating prims
    ActionPositions positions(this->htn, this->plan);
    PrimsForMatching primsForMatching(this->htn, 
                                      this->h.getNumPrims(),
                                      propsForMatching,
                                      positions);
    this->h.addPrims(primsForMatching.get());
    PrimsForCounters primsForGlobalCounter(propsForGlobalCounter, 
                                           this->h.getNumPrims());
    this->h.addPrims(primsForGlobalCounter.get());
}