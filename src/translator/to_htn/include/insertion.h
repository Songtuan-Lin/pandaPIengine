#include "Model.h"
#include "util.h"
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
    
    public:
        PrimInsertion(Slot s, Proposition empty, Proposition occupied);
};