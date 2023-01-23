#ifndef _insertion_inc_h_
#define _insertion_inc_h_

#include "Model.h"
#include "match.h"
#include "accumulation.h"
#include "meta.h"
#include "task.h"
#include "method.h"
#include "counter.h"

class PropsForInsertion {
    private:
        Proposition propEmpty, propOccupied;
    
    public:
        PropsForInsertion(Slot s, int id) {
            string name = "empty" + s.toString();
            this->propEmpty = Proposition(name, id);
            assert(this->propEmpty.getID() == id);
            name = "occupied" + s.toString();
            this->propOccupied = Proposition(name, id + 1);
            assert(this->propOccupied.getID() == id + 1);
        }
        Proposition getEmpty() {
            assert(this->propEmpty.validate());
            return this->propEmpty;
        }
        Proposition getOccupied() {
            assert(this->propOccupied.validate());
            return this->propOccupied;
        }
        vector<Proposition> get() {
            assert(this->propOccupied.validate() && this->propEmpty.validate());
            return {this->propEmpty, this->propOccupied};
        }
};

class PrimsForInsertion {
    private:
        vector<PrimitiveTask> prims;
        Slot s;
    
    public:
        PrimsForInsertion(
                Slot s, int startID,
                PropsForMatching &propsForMatching,
                PropsForInsertion &propsForInsertion, 
                const vector<int> &positions);
        PrimitiveTask getEmpty() {return this->prims[0];}
        vector<PrimitiveTask> getOccupied() {
            vector<PrimitiveTask> occupied(this->prims.begin() + 1, this->prims.end());
            return occupied;
        }
        vector<PrimitiveTask> get() {return this->prims;}
};

class CompForInsertion {
    private:
        CompoundTask c;
        Slot s;
    
    public:
        CompForInsertion(Slot s, int id) {
            this->s = s;
            string name = "slot" + s.toString();
            CompoundTask c(name, id);
            this->c = c;
            assert(this->c.validate());
        }
        CompoundTask get() {return this->c;}
};

class MethodsForInsertion {
    private:
        vector<Method> methods;
        Slot s;
    
    public:
        MethodsForInsertion(
                Slot s, 
                CompForInsertion compIns, 
                PrimsForInsertion primIns,
                Counter global,
                Counter local);
        vector<Method> get() {return this->methods;}
};

#endif