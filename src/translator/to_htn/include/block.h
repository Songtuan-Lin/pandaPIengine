#ifndef _block_inc_h_
#define _block_inc_h_

#include "meta.h"
#include "task.h"
#include "method.h"
#include "counter.h"
#include "insertion.h"

class MethodStartingPrim {
    private:
        PrimitiveTask prim;
    
    public:
        MethodStartingPrim(Counter local, int id, int m) {
            string name = "start[" + to_string(m) + "]";
            PrimitiveTask prim(0, {}, {local.props.getInit()}, {}, name, id);
            this->prim = prim;
        }
        PrimitiveTask get() {return this->prim;}
};

#endif