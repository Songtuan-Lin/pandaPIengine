#include "Model.h"
#include "meta.h"
#include "task.h"
#include "method.h"

class PropInsertion {
    private:
        Proposition propEmpty, propOccupied;
    
    public:
        PropInsertion(Slot s, int id) {
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
};

class PrimInsertion {
    private:
        vector<PrimitiveTask> prims;
        Slot s;
    
    public:
        PrimInsertion(
                Slot s, int startID,
                const Proposition &init,
                const Proposition &empty, 
                const Proposition &occupied, 
                const vector<int> &positions, 
                const vector<Proposition> &props);
};

class CompInsertion {
    private:
        CompoundTask c;
        Slot s;
    
    public:
        CompInsertion(Slot s, int id) {
            this->s = s;
            string name = "slot" + s.toString();
            CompoundTask c(name, id);
            this->c = c;
            assert(this->c.validate());
        }
        CompoundTask getTask() {return this->c;}
};

class MethodInsertion {
    private:
        vector<Method> methods;
        Slot s;
    
    public:
        MethodInsertion(Slot s, CompoundTask c, PrimInsertion primIns);
};