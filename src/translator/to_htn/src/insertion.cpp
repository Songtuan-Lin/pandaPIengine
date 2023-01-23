#include "insertion.h"

PrimsForInsertion::PrimsForInsertion(
        Slot s, int startID,
        PropsForMatching &propsForMatching,
        PropsForInsertion &propsForInsertion, 
        const vector<int> &positions) {
    this->s = s;
    int id = startID;
    string name = "empty" + s.toString();
    Proposition empty = propsForInsertion.getEmpty();
    Proposition occupied = propsForInsertion.getOccupied();
    vector<Proposition> props = propsForMatching.getMatchingProps();
    vector<Proposition> prec = {empty};
    vector<Proposition> del = {occupied};
    vector<Proposition> add;
    assert(add.size() == 0);
    PrimitiveTask prim(0, prec, add, del, name, id);
    id += 1;
    prims.push_back(prim);
    for (int pos : positions) {
        if (s.i < pos) continue;
        Proposition prev;
        if (pos == 0) {
            prev = propsForMatching.getInit();
        } else {prev = props[pos - 1];}
        string name = "match_to_" + to_string(pos) + s.toString();
        vector<Proposition> prec = {occupied, prev};
        vector<Proposition> del = {empty, prev};
        vector<Proposition> add = {props[pos]};
        PrimitiveTask prim(1, prec, add, del, name, id);
        prims.push_back(prim);
        id++;
    }
}

MethodsForInsertion::MethodsForInsertion(
        Slot s, 
        CompForInsertion compIns, 
        PrimsForInsertion primIns, 
        Counter global, 
        Counter local) {
    string name = "not_occupy" + s.toString();
    TaskNetwork tn({primIns.getEmpty()});
    Method m(name, compIns.get(), tn);
    this->methods.push_back(m);
    vector<PrimitiveTask> prims = primIns.getOccupied();
    for (PrimitiveTask prim : prims) {
        name = "occupy" + s.toString();
        TaskNetwork tn(
            {global.compForCounter->get(), 
             local.compForCounter->get(), 
             prim}, true);
        Method m(name, compIns.get(), tn);
        this->methods.push_back(m);
    }
}