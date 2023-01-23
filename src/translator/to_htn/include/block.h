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
            PrimitiveTask prim(0, {}, {local.propsForCounter->getInit()}, {}, name, id);
            this->prim = prim;
        }
        PrimitiveTask get() {return this->prim;}
};

#endif