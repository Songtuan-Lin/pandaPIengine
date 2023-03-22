#include "catch.hpp"
#include "match.h"
#include "insertion.h"
#include "to_htn.h"

TEST_CASE("TEST PROPOSITIONS FOR MATCHING") {
    PropsForMatching propsForMatching(3, 0);
    vector<Proposition> props = propsForMatching.get();
    REQUIRE(props.size() == 4);
    for (int i = 0; i < props.size(); i++)
        REQUIRE(props[i].getID() == i);
    int idForInit = propsForMatching.getInit().getID();
    REQUIRE(idForInit == props[0].getID());
    vector<Proposition> matching = propsForMatching.getMatchingProps();
    REQUIRE(props.size() == matching.size() + 1);
    for (int i = 0; i < matching.size(); i++)
        REQUIRE(matching[i].getID() == props[i + 1].getID());
}

TEST_CASE("TEST PROPOSITIONS FOR INSERTIONS") {
    Slot slot(0, 1, 1);
    PropsForInsertion propsForInsertion(slot, 4);
    REQUIRE(propsForInsertion.getEmpty().getID() == 4);
    REQUIRE(propsForInsertion.getOccupied().getID() == 5);
    vector<Proposition> props = propsForInsertion.get();
    REQUIRE(props.size() == 2);
    REQUIRE(props[0].getID() == 4);
    REQUIRE(props[1].getID() == 5);
}

TEST_CASE("TEST PRIMS FOR INSERTIONS") {
    // vector<int> plan = {2, 2, 1};
    vector<int> positions = {0, 1};
    PropsForMatching propsForMatching(3, 0);
    Slot slot(0, 1, 1);
    PropsForInsertion propsForInsertion(slot, 4);
    Proposition occupied = propsForInsertion.getOccupied();
    Proposition empty = propsForInsertion.getEmpty();
    PrimsForInsertion primsForInsertion(slot, 0,
                                        propsForMatching,
                                        propsForInsertion,
                                        positions);
    vector<PrimitiveTask> prims = primsForInsertion.get();
    REQUIRE(prims.size() == 2);
    REQUIRE(prims[0].getID() == 0);
    REQUIRE(prims[0].getID() == primsForInsertion.getEmpty().getID());
    REQUIRE(prims[1].getID() == 1);
    REQUIRE(primsForInsertion.getOccupied().size() == 1);
    REQUIRE(prims[1].getID() == primsForInsertion.getOccupied()[0].getID());
    // test preconditions and effects for actions
    // representing not insertions
    vector<Proposition> precond = prims[0].getPrecondition();
    REQUIRE(precond.size() == 1);
    REQUIRE(precond[0].getID() == empty.getID());
    vector<Proposition> posEffs = prims[0].getPosEffs();
    REQUIRE(posEffs.size() == 0);
    vector<Proposition> negEffs = prims[0].getNegEffs();
    REQUIRE(negEffs.size() == 1);
    REQUIRE(negEffs[0].getID() == occupied.getID());
    // test preconditions and effects for actions
    // representing insertions
    precond = prims[1].getPrecondition();
    REQUIRE(precond.size() == 2);
    REQUIRE(precond[0].getID() == occupied.getID());
    REQUIRE(precond[1].getID() == propsForMatching.getProp(0).getID());
    posEffs = prims[1].getPosEffs();
    REQUIRE(posEffs.size() == 1);
    REQUIRE(posEffs[0].getID() == propsForMatching.getProp(1).getID());
    negEffs = prims[1].getNegEffs();
    REQUIRE(negEffs.size() == 2);
    REQUIRE(negEffs[0].getID() == empty.getID());
    REQUIRE(negEffs[1].getID() == propsForMatching.getProp(0).getID());
}

TEST_CASE("TEST COUNTERS") {
    PropsForCounter propsForCounter(2, 0);
    REQUIRE(propsForCounter.get().size() == 4);
    vector<Proposition> props = propsForCounter.get();
    REQUIRE(props[0].getID() == propsForCounter.getInit().getID());
    // test prims for counting
    PrimsForCounter primsForCounter(propsForCounter, 0);
    REQUIRE(primsForCounter.get().size() == 3);
    vector<PrimitiveTask> prims = primsForCounter.get();
    // test counting 1
    vector<Proposition> prec = prims[0].getPrecondition();
    REQUIRE(prec.size() == 1);
    REQUIRE(prec[0].getID() == propsForCounter.getInit().getID());
    vector<Proposition> posEffs = prims[0].getPosEffs();
    REQUIRE(posEffs.size() == 1);
    REQUIRE(posEffs[0].getID() == props[1].getID());
    vector<Proposition> negEffs = prims[0].getNegEffs();
    REQUIRE(negEffs.size() == 1);
    REQUIRE(negEffs[0].getID() == props[0].getID());
    // test counting 2
    prec = prims[1].getPrecondition();
    REQUIRE(prec.size() == 1);
    REQUIRE(prec[0].getID() == props[1].getID());
    posEffs = prims[1].getPosEffs();
    REQUIRE(posEffs.size() == 1);
    REQUIRE(posEffs[0].getID() == props[2].getID());
    negEffs = prims[1].getNegEffs();
    REQUIRE(negEffs.size() == 1);
    REQUIRE(negEffs[0].getID() == props[1].getID());
    CompForCounter compForCounter(2);
    MethodsForCounter methodsForCounter(compForCounter, 
                                        primsForCounter);
    vector<Method> methods = methodsForCounter.get();
    REQUIRE(methods.size() == 3);
    Task c = methods[0].getDecomposedTask();
    REQUIRE(c.getID() == compForCounter.get().getID());
    TaskNetwork tn = methods[0].getTaskNetwork();
    vector<Task> tasks = tn.getTasks();
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks[0].getID() == prims[0].getID());
    c = methods[1].getDecomposedTask();
    REQUIRE(c.getID() == compForCounter.get().getID());
    tn = methods[1].getTaskNetwork();
    tasks = tn.getTasks();
    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks[0].getID() == prims[1].getID());
}

TEST_CASE("TEST METHODS FOR INSERTIONS") {
    vector<int> positions = {0, 1};
    PropsForMatching propsForMatching(3, 0);
    Slot slot(0, 1, 1);
    // counter for 0 -- 2
    Counter global;
    PropsForCounter propsForGlobalCounter(2, 4);
    PrimsForCounter primsForGlobalCounter(propsForGlobalCounter, 0);
    CompForCounter compForGlobalCounter(5);
    global.propsForCounter = propsForGlobalCounter;
    global.primsForCounter = primsForGlobalCounter;
    global.compForCounter = compForGlobalCounter;
    Counter local;
    // counter for 0 -- 1
    PropsForCounter propsForLocalCounter(1, 7);
    PrimsForCounter primsForLocalCounter(propsForLocalCounter, 2);
    CompForCounter compForLocalCounter(6);
    local.propsForCounter = propsForLocalCounter;
    local.primsForCounter = primsForLocalCounter;
    local.compForCounter = compForLocalCounter;
    // insertions
    PropsForInsertion propsForInsertion(slot, 9);
    PrimsForInsertion primsForInsertion(slot, 3,
                                        propsForMatching,
                                        propsForInsertion,
                                        positions);
    vector<PrimitiveTask> prims = primsForInsertion.get(); // size == 2
    CompForInsertion compForInsertion(slot, 7);
    REQUIRE(compForInsertion.get().getID() == 7);
    MethodsForInsertion methodsForInsertion(slot, 
                                            compForInsertion,
                                            primsForInsertion,
                                            global, local);
    vector<Method> methods = methodsForInsertion.get();
    REQUIRE(methods.size() == 2);
    Method m = methods[0];
    Task c = m.getDecomposedTask();
    REQUIRE(c.getID() == compForInsertion.get().getID());
    TaskNetwork tn = m.getTaskNetwork();
    REQUIRE(tn.getTasks().size() == 1);
    REQUIRE(tn.getTasks()[0].getID() == 3);
    REQUIRE(tn.getTasks()[0].getID() == primsForInsertion.getEmpty().getID());
    m = methods[1];
    c = m.getDecomposedTask();
    REQUIRE(c.getID() == compForInsertion.get().getID());
    tn = m.getTaskNetwork();
    REQUIRE(tn.getTasks().size() == 3);
    vector<Task> tasks = tn.getTasks();
    REQUIRE(tasks[0].getID() == 5);
    REQUIRE(tasks[0].getID() == global.compForCounter.get().getID());
    REQUIRE(tasks[1].getID() == 6);
    REQUIRE(tasks[1].getID() == local.compForCounter.get().getID());
    REQUIRE(tasks[2].getID() == 4);
    REQUIRE(tasks[2].getID() == primsForInsertion.get()[1].getID());
}

TEST_CASE("TEST TRANSLATION") {
    string htnFile = "/home/users/u6162630/Datasets/htn-to-inval-noprec/Depots/p05/plan_1/p05-planning.sas";
    string planFile = "/home/users/u6162630/Datasets/htn-to-inval-noprec/Depots/p05/plan_1/plan-cyk.txt";
    string oFilePath = "/home/users/u6162630/Projects/ongoing/pandaPIengine/src/translator/test/to_htn/output.sas";
    ofstream ofile(oFilePath);
    HTNTranslator *translator = new HTNTranslator(htnFile, planFile);
    translator->write(ofile);
}