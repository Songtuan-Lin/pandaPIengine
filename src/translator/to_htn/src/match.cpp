#include "match.h"

PrimsForMatching::PrimsForMatching(
        Model *htn, int startID,
        PropsForMatching &props,
        ActionPositions &positions) {
    this->lookup.resize(htn->numActions);
    int id = startID;
    for (int a = 0; a < htn->numActions; a++) {
        for (int pos : positions.get(a)) {
            string name = "matched[" + to_string(pos) + "]";
            vector<Proposition> prec, del;
            if (pos == 0) {
                prec = {props.getInit()};
                del = {props.getInit()};
            } else {
                prec = {props.getProp(pos - 1)};
                del = {props.getProp(pos - 1)};
            }
            vector<Proposition> add = {props.getProp(pos)};
            PrimitiveTask prim(0, prec, add, del, name, id);
            this->prims.push_back(prim);
            this->lookup[a].push_back(prim);
            id++;
        }
    }
}

MethodsForMatching::MethodsForMatching(
        Model *htn, 
        PrimsTranslation &translation, 
        PrimsForMatching &primsForMatching) {
    for (int a = 0; a < htn->numActions; a++) {
        CompoundTask c = translation.get(a);
        if (!c.validate()) continue;
        for (PrimitiveTask &p : primsForMatching.get(a)) {
            string name = "matching[" + to_string(a) + "]";
            TaskNetwork tn({p});
            Method m(name, c, tn);
            this->methods.push_back(m);
        }
    }
}