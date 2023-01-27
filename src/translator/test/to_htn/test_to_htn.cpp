#include "catch.hpp"
#include "match.h"
#include "insertion.h"

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