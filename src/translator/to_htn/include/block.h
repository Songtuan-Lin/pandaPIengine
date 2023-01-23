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

class MethodForBlock {
    private:
        Method m;
    
    public:
        MethodForBlock(
                Block block,
                CompForBlock compForBlock,
                vector<vector<vector<SlotTranslation>>> &translations) {
            string name = "decompose" + block.toString();
            vector<Task> tasks;
            for (SlotTranslation &translation : translations[block.m][block.b]) {
                if(translation.compForInsertion == nullptr) continue;
                tasks.push_back(translation.compForInsertion->get());
            }
            TaskNetwork tn(tasks, true);
            this->m = Method(name, compForBlock.get(), tn);
        }
        Method get() {return this->m;}
};

struct BlockTranslation {
    CompForBlock *compForBlock = nullptr;
};

#endif