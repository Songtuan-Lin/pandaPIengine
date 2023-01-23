#ifndef _block_inc_h_
#define _block_inc_h_

#include "meta.h"
#include "task.h"
#include "method.h"
#include "counter.h"
#include "insertion.h"

class PrimForStartingMethod {
    private:
        PrimitiveTask prim;
    
    public:
        PrimForStartingMethod(Counter local, int id, int m) {
            string name = "start[" + to_string(m) + "]";
            Proposition init = local.propsForCounter->getInit();
            this->prim = PrimitiveTask(0, {}, {init}, {}, name, id);
            assert(this->prim.validate());
        }
        PrimitiveTask get() {return this->prim;}
};

class CompForBlock {
    private:
        CompoundTask c;
    
    public:
        CompForBlock(Block b, int id) {
            string name = "block" + b.toString();
            this->c = CompoundTask(name, id);
            assert(this->c.validate());
        }
        CompoundTask get() {return this->c;}
};

#endif