#include "to_htn.h"

HTNTranslator::HTNTranslator(string htnFile, string planFile) : Translator(htnFile, planFile) {
    this->countersForMethods.resize(this->htn->numMethods);
    this->startingPrimsForMethods.resize(this->htn->numMethods);
    int numCounter = 0;
    Counter global;
    // creating propositions and prims
    cout << "[Generate props for matching]";
    PropsForMatching propsForMatching(this->plan.size(), 
                                      this->h.getNumProps());
    cout << " Num props generated: "; 
    cout << propsForMatching.get().size() << endl;
    this->h.addProps(propsForMatching.get());
    this->h.addInitState(propsForMatching.getInit());
    cout << "[Generate prims for matching]";
    ActionPositions positions(this->htn, this->plan);
    PrimsForMatching primsForMatching(this->htn, 
                                      this->h.getNumPrims(),
                                      propsForMatching,
                                      positions);
    cout << " Num prims generated: ";
    cout << primsForMatching.get().size() << endl;
    this->h.addPrims(primsForMatching.get());
    int maxInsertions = this->plan.size() - traversal->getNumReachable(this->htn->initialTask);
    cout << "[Generate props for the global counter]";
    PropsForCounter propsForGlobalCounter(maxInsertions, 
                                          this->h.getNumProps());
    cout << " Num props generated: ";
    cout << propsForGlobalCounter.get().size() << endl;
    global.propsForCounter = propsForGlobalCounter; 
    this->h.addProps(propsForGlobalCounter.get());
    this->h.addInitState(propsForGlobalCounter.getInit());
    cout << "[Generate prims for the global counter]";
    PrimsForCounter primsForGlobalCounter(propsForGlobalCounter, 
                                          this->h.getNumPrims());
    cout << " Num prims generated: ";
    cout << primsForGlobalCounter.get().size() << endl;
    global.primsForCounter = primsForGlobalCounter;
    this->h.addPrims(primsForGlobalCounter.get());
    this->slotTranslations.resize(this->htn->numMethods);
    string prefix = "[Generate props and prims for slots]";
    int numProps = 0, numPrims = 0;
    for (int m = 0; m < this->htn->numMethods; m++) {
        string progress = to_string(m + 1) + "/" + to_string(this->htn->numMethods);
        cout << "\r" << prefix << " " << progress << " methods visited";
        cout.flush();
        if (this->optimizeHTN->isMethodInvalid(m)) continue;
        int minReachableTasks = 0;
        for (int t = 0; t < this->htn->numSubTasks[m]; t++)
            minReachableTasks += traversal->getNumReachable(t);
        int bound = this->plan.size() - minReachableTasks;
        PropsForCounter propsForLocalCounter(bound, this->h.getNumProps());
        numProps += propsForLocalCounter.get().size();
        this->h.addProps(propsForLocalCounter.get());
        this->countersForMethods[m].propsForCounter = propsForLocalCounter;
        // prims for each method
        PrimsForCounter primsForLocalCounter(propsForLocalCounter, 
                                             this->h.getNumPrims());
        this->countersForMethods[m].primsForCounter = primsForLocalCounter;
        numPrims += primsForLocalCounter.get().size();
        this->h.addPrims(primsForLocalCounter.get());
        PrimForStartingMethod primForStartingMethod(this->countersForMethods[m],
                                                    this->h.getNumPrims(),
                                                    m);
        numPrims += 1;
        this->h.addPrims(primForStartingMethod.get());
        this->startingPrimsForMethods[m] = primForStartingMethod;
        this->slotTranslations[m].resize(this->htn->numSubTasks[m] + 1);
        for (int b = 0; b < this->htn->numSubTasks[m] + 1; b++) {
            this->slotTranslations[m][b].resize(this->plan.size());
            for (int i = 0; i < this->plan.size(); i++) {
                if (!this->validation->isValid(m, b, i)) continue;
                Slot s(m, b, i);
                PropsForInsertion propsForInsertion(s, this->h.getNumProps());
                numProps += propsForInsertion.get().size();
                this->h.addProps(propsForInsertion.get());
                this->h.addInitState(propsForInsertion.getEmpty());
                this->h.addInitState(propsForInsertion.getOccupied());
                this->slotTranslations[m][b][i].propsForInsertion = propsForInsertion;
                PrimsForInsertion primsForInsertion(s, this->h.getNumPrims(),
                                                    propsForMatching,
                                                    propsForInsertion,
                                                    positions.get(this->plan[i]));
                numPrims += primsForInsertion.get().size();
                this->h.addPrims(primsForInsertion.get());
                this->slotTranslations[m][b][i].primsForInsertion = primsForInsertion;
            }
        }
    }
    cout << endl;
    cout << prefix << " Num props generated: " << numProps << endl;
    cout << prefix << " Num prims generated: " << numPrims << endl;
    this->h.addGoal(propsForMatching.getProp(this->plan.size() - 1));
    // creating compound tasks and methods
    int offset = this->h.getNumPrims();
    // TODO: add the counter id
    cout << "[Generate comp for the global counter]";
    CompForCounter compForGlobalCounter(offset + this->h.getNumComps(), numCounter);
    cout << " Done!" << endl;
    numCounter++;
    global.compForCounter = compForGlobalCounter;
    this->h.addComps(compForGlobalCounter.get());
    cout << "[Generate methods for the global counter]";
    MethodsForCounter methodsForGlobalCounter(compForGlobalCounter,
                                              primsForGlobalCounter);
    cout << " Num methods generated: ";
    cout << methodsForGlobalCounter.get().size() << endl;
    this->h.addMethods(methodsForGlobalCounter.get());
    cout << "[Translate comps]";
    CompsTranslation compsTranslation(this->htn, 
                                    offset + this->h.getNumComps(), 
                                    this->optimizeHTN);
    cout << " Num comps generated: ";
    cout << compsTranslation.get().size() << endl;
    this->h.addComps(compsTranslation.get());
    ActionAccumulation accumulation(this->htn, this->plan);
    cout << "[Translate prims]";
    PrimsTranslation primsTranslation(this->htn, 
                                      offset + this->h.getNumComps(), 
                                      accumulation);
    cout << " Num comps generated: ";
    cout << primsTranslation.get().size() << endl;
    this->h.addComps(primsTranslation.get());
    cout << "[Generate methods for matching]";
    MethodsForMatching methodsForMatching(this->htn, 
                                          primsTranslation, 
                                          primsForMatching,
                                          accumulation);
    cout << " Num methods generated: ";
    cout << methodsForMatching.get().size() << endl;
    this->h.addMethods(methodsForMatching.get());
    this->blockTranslations.resize(this->htn->numMethods);
    prefix = "[Generate comps and methods for each slot]";
    int numComps = 0, numMethods = 0;
    for (int m = 0; m < this->htn->numMethods; m++) {
        string progress = to_string(m + 1) + "/" + to_string(this->htn->numMethods);
        cout << "\r" << prefix << " " << progress << " methods visited";
        cout.flush();
        if (this->optimizeHTN->isMethodInvalid(m)) continue;
        CompForCounter compForLocalCounter(offset + this->h.getNumComps(), numCounter);
        numCounter++;
        this->countersForMethods[m].compForCounter = compForLocalCounter;
        this->h.addComps(compForLocalCounter.get());
        MethodsForCounter methodsForLocalCounter(
                compForLocalCounter,
                this->countersForMethods[m].primsForCounter);
        numMethods += methodsForLocalCounter.get().size();
        this->h.addMethods(methodsForLocalCounter.get());
        this->blockTranslations[m].resize(this->htn->numSubTasks[m] + 1);
        for (int b = 0; b < this->htn->numSubTasks[m] + 1; b++) {
            bool isValidBlock = false;
            for (int i = 0; i < this->plan.size(); i++) {
                if (!this->validation->isValid(m, b, i)) continue;
                isValidBlock = true;
                Slot s(m, b, i);
                CompForInsertion compForInsertion(s, offset + this->h.getNumComps());
                numComps += 1;
                this->slotTranslations[m][b][i].compForInsertion = compForInsertion;
                this->h.addComps(compForInsertion.get());
                MethodsForInsertion methodsForInsertion(
                        s, compForInsertion, 
                        this->slotTranslations[m][b][i].primsForInsertion,
                        global, this->countersForMethods[m]);
                numMethods += methodsForInsertion.get().size();
                this->h.addMethods(methodsForInsertion.get());
            }
            if(!isValidBlock) continue;
            CompForBlock compForBlock(Block(m, b), this->h.getNumComps() + offset);
            numComps += 1;
            this->h.addComps(compForBlock.get());
            this->blockTranslations[m][b].compForBlock = compForBlock;
            this->blockTranslations[m][b].valid = true;
            MethodForBlock methodForBlock(Block(m, b),
                                          compForBlock,
                                          this->slotTranslations);
            numMethods += 1;
            this->h.addMethods(methodForBlock.get());
        }
        MethodTranslation methodTranslation(this->htn, m, 
                                            primsTranslation, 
                                            compsTranslation, 
                                            this->blockTranslations[m], 
                                            this->startingPrimsForMethods[m]);
        numMethods += 1;
        this->h.addMethods(methodTranslation.get());
    }
    cout << endl;
    cout << prefix << " Num comps generated: " << numComps << endl;
    cout << prefix << " Num methods generated: " << numMethods << endl;
    this->h.setTopTask(compsTranslation.get(this->htn->initialTask));
#ifndef NDEBUG
    assert(this->h.validate());
#endif
}