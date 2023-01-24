#include "to_htn.h"

HTNTranslator::HTNTranslator(string htnFile, string planFile) : Translator(htnFile, planFile) {
    this->countersForMethods.resize(this->htn->numMethods);
    this->startingPrimsForMethods.resize(this->htn->numMethods);
    Counter global;
    // creating propositions and prims
    PropsForMatching propsForMatching(this->plan.size(), 
                                      this->h.getNumProps());
    this->h.addProps(propsForMatching.get());
    ActionPositions positions(this->htn, this->plan);
    PrimsForMatching primsForMatching(this->htn, 
                                      this->h.getNumPrims(),
                                      propsForMatching,
                                      positions);
    this->h.addPrims(primsForMatching.get());
    int maxInsertions = this->plan.size() - traversal->getNumReachable(this->htn->initialTask);
    PropsForCounter propsForGlobalCounter(maxInsertions, 
                                          this->h.getNumProps());
    global.propsForCounter = propsForGlobalCounter; 
    this->h.addProps(propsForGlobalCounter.get());
    PrimsForCounter primsForGlobalCounter(propsForGlobalCounter, 
                                          this->h.getNumPrims());
    global.primsForCounter = primsForGlobalCounter;
    this->h.addPrims(primsForGlobalCounter.get());
    this->slotTranslations.resize(this->htn->numMethods);
    for (int m = 0; m < this->htn->numMethods; m++) {
        if (this->optimizeHTN->isMethodInvalid(m)) continue;
        int minReachableTasks = 0;
        for (int t = 0; t < this->htn->numSubTasks[m]; t++)
            minReachableTasks += traversal->getNumReachable(t);
        int bound = this->plan.size() - minReachableTasks;
        PropsForCounter propsForLocalCounter(bound, this->h.getNumProps());
        this->h.addProps(propsForLocalCounter.get());
        this->countersForMethods[m].propsForCounter = propsForLocalCounter;
        // prims for each method
        PrimsForCounter primsForLocalCounter(propsForLocalCounter, 
                                             this->h.getNumPrims());
        this->countersForMethods[m].primsForCounter = primsForLocalCounter;
        this->h.addPrims(primsForLocalCounter.get());
        PrimForStartingMethod primForStartingMethod(this->countersForMethods[m],
                                                    this->h.getNumPrims(),
                                                    m);
        this->h.addPrims(primForStartingMethod.get());
        this->startingPrimsForMethods[m] = primForStartingMethod;
        this->slotTranslations[m].resize(this->htn->numSubTasks[m] + 1);
        for (int b = 0; b < this->htn->numSubTasks[m] + 1; b++) {
            this->slotTranslations[m][b].resize(this->plan.size());
            for (int i = 0; i < this->plan.size(); i++) {
                if (!this->validation->isValid(m, b, i)) continue;
                Slot s(m, b, i);
                PropsForInsertion propsForInsertion(s, this->h.getNumProps());
                this->h.addProps(propsForInsertion.get());
                this->slotTranslations[m][b][i].propsForInsertion = propsForInsertion;
                PrimsForInsertion primsForInsertion(s, this->h.getNumPrims(),
                                                    propsForMatching,
                                                    propsForInsertion,
                                                    positions.get(this->plan[i]));
                this->h.addPrims(primsForInsertion.get());
                this->slotTranslations[m][b][i].primsForInsertion = primsForInsertion;
            }
        }
    }
    // creating compound tasks and methods
    int offset = this->h.getNumPrims();
    // TODO: add the counter id
    CompForCounter compForGlobalCounter(offset + this->h.getNumComps());
    global.compForCounter = compForGlobalCounter;
    this->h.addComps(compForGlobalCounter.get());
    MethodsForCounter methodsForGlobalCounter(compForGlobalCounter,
                                              primsForGlobalCounter);
    this->h.addMethods(methodsForGlobalCounter.get());
    CompsTranslation compsTranslation(this->htn, 
                                    offset + this->h.getNumComps(), 
                                    this->optimizeHTN);
    this->h.addComps(compsTranslation.get());
    ActionAccumulation accumulation(this->htn, this->plan);
    PrimsTranslation primsTranslation(this->htn, 
                                      this->h.getNumComps(), 
                                      accumulation);
    this->h.addComps(primsTranslation.get());
    MethodsForMatching methodsForMatching(this->htn, 
                                          primsTranslation, 
                                          primsForMatching);
    this->h.addMethods(methodsForMatching.get());
    this->blockTranslations.resize(this->htn->numMethods);
    for (int m = 0; m < this->htn->numMethods; m++) {
        if (this->optimizeHTN->isMethodInvalid(m)) continue;
        CompForCounter compForLocalCounter(offset + this->h.getNumComps());
        this->countersForMethods[m].compForCounter = compForLocalCounter;
        this->h.addComps(compForLocalCounter.get());
        MethodsForCounter methodsForLocalCounter(
                compForLocalCounter,
                this->countersForMethods[m].primsForCounter);
        this->h.addMethods(methodsForLocalCounter.get());
        this->blockTranslations[m].resize(this->htn->numSubTasks[m] + 1);
        for (int b = 0; b < this->htn->numSubTasks[m] + 1; b++) {
            CompForBlock compForBlock(Block(m, b), this->h.getNumComps());
            this->h.addComps(compForBlock.get());
            this->blockTranslations[m][b].compForBlock = compForBlock;
            for (int i = 0; i < this->plan.size(); i++) {
                if (!this->validation->isValid(m, b, i)) continue;
                Slot s(m, b, i);
                CompForInsertion compForInsertion(s, offset + this->h.getNumComps());
                this->slotTranslations[m][b][i].compForInsertion = compForInsertion;
                this->h.addComps(compForInsertion.get());
                MethodsForInsertion methodsForInsertion(
                        s, compForInsertion, 
                        this->slotTranslations[m][b][i].primsForInsertion,
                        global, this->countersForMethods[m]);
                this->h.addMethods(methodsForInsertion.get());
            }
            MethodForBlock methodForBlock(Block(m, b),
                                          compForBlock,
                                          this->slotTranslations);
            this->h.addMethods(methodForBlock.get());
        }
        MethodTranslation methodTranslation(this->htn, m, 
                                            primsTranslation, 
                                            compsTranslation, 
                                            this->blockTranslations[m], 
                                            this->startingPrimsForMethods[m]);
        this->h.addMethods(methodTranslation.get());
    }
}