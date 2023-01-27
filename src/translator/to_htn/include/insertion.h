#ifndef _insertion_inc_h_
#define _insertion_inc_h_

#include "Model.h"
#include "match.h"
#include "accumulation.h"
#include "task.h"
#include "method.h"
#include "counter.h"

struct Slot {
    int m, b, i;
    Slot() {this->m = -1; this->b = -1; this->i = -1;}
    Slot(int m, int b, int i) {this->m = m; this->b = b; this->i = i;}
    std::string toString() {
        return "[" + std::to_string(m) + ";" + std::to_string(b) + ";" + std::to_string(i) + "]";
    }
};

struct Block {
    int m, b;
    Block() {this->m = -1; this->b = -1;}
    Block(int m, int b) {this->m = m; this->b = b;}
    std::string toString() {
        return "[" + std::to_string(m) + ";" + std::to_string(b) + "]";
    }
};

class PropsForInsertion {
    private:
        Proposition propEmpty, propOccupied;
    
    public:
        PropsForInsertion() {}
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
        bool validate() {
            return this->propEmpty.validate() && this->propOccupied.validate();
        }
};

class PrimsForInsertion {
    private:
        vector<PrimitiveTask> prims;
        Slot s;
    
    public:
        PrimsForInsertion() {}
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
        vector<PrimitiveTask> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->prims;
        }
        bool validate() {
            for (PrimitiveTask &p : this->prims)
                if (!p.validate()) return false;
            return true;
        }
};

class CompForInsertion {
    private:
        CompoundTask c;
        Slot s;
    
    public:
        CompForInsertion() {}
        CompForInsertion(Slot s, int id) {
            this->s = s;
            string name = "slot" + s.toString();
            CompoundTask c(name, id);
            this->c = c;
            assert(this->c.validate());
        }
        CompoundTask get() {
            assert(this->validate());
            return this->c;
        }
        bool validate() {return this->c.validate();}
};

class MethodsForInsertion {
    private:
        vector<Method> methods;
        Slot s;
    
    public:
        MethodsForInsertion() {}
        MethodsForInsertion(
                Slot s, 
                CompForInsertion compIns, 
                PrimsForInsertion primIns,
                Counter global,
                Counter local);
        vector<Method> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->methods;
        }
        bool validate() {
            for (Method &m : this->methods) 
                if (!m.validate()) return false;
            return true;
        }
};

#endif