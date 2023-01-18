#include "insertion.h"

PrimInsertion::PrimInsertion(
        Slot s, int startID,
        const Proposition &init,
        const Proposition &empty, 
        const Proposition &occupied, 
        const vector<int> &positions, 
        const vector<Proposition> &props) {
    this->s = s;
    int id = startID;
    string name = "empty" + s.toString();
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
            prev = init;
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