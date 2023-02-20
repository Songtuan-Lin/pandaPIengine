#ifndef _block_inc_h_
#define _block_inc_h_

#include <cmath>
#include "meta.h"
#include "task.h"
#include "method.h"
#include "counter.h"
#include "insertion.h"

class PrimForStartingMethod {
    private:
        PrimitiveTask prim;
        bool valid = false;
    
    public:
        PrimForStartingMethod() {}
        PrimForStartingMethod(Counter local, int id, int m) {
            this->valid = true;
            string name = "start[" + to_string(m) + "]";
            Proposition init = local.propsForCounter.getInit();
            this->prim = PrimitiveTask(0, {}, {init}, {}, name, id);
            assert(this->prim.validate());
        }
        PrimitiveTask get() {
            assert(this->validate());
            return this->prim;
        }
        bool validate() {return this->prim.validate();}
        bool isValid() {return this->valid;}
};

class CompForBlock {
    private:
        CompoundTask c;
    
    public:
        CompForBlock() {}
        CompForBlock(Block b, int id) {
            string name = "block" + b.toString();
            this->c = CompoundTask(name, id);
            assert(this->c.validate());
        }
        CompoundTask get() {
            assert(this->validate());
            return this->c;
        }
        bool validate() {return this->c.validate();}
};

class MethodForBlock {
    private:
        Method m;
    
    public:
        MethodForBlock() {}
        MethodForBlock(
                Block block,
                CompForBlock compForBlock,
                vector<vector<vector<SlotTranslation>>> &translations) {
            string name = "decompose" + block.toString();
            vector<Task> tasks;
            for (SlotTranslation &translation : translations[block.m][block.b]) {
                if(!translation.compForInsertion.validate()) continue;
                tasks.push_back(translation.compForInsertion.get());
            }
            TaskNetwork tn(tasks, true);
            this->m = Method(name, compForBlock.get(), tn);
        }
        Method get() {return this->m;}
        bool validate() {return m.validate();}
};

struct BlockTranslation {
    CompForBlock compForBlock;
    bool valid = false;
};

#endif