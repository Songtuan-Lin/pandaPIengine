#include "meta.h"
#include "task.h"
#include "method.h"
#include "counter.h"
#include "insertion.h"

class BlockPrim {
    private:
        PrimitiveTask prim;
    
    public:
        BlockPrim(Block b, Counter local, int id) {
            string name = "start" + b.toString();
            PrimitiveTask start(0, {}, {local.props.getInit()}, {}, name, id);
            this->prim = start;
        }
        PrimitiveTask getTask() {return this->prim;}
};

class BlockComp {
    private:
        CompoundTask c;
    
    public:
        BlockComp(Block b, int id) {
            string name = "block" + b.toString();
            CompoundTask c(name, id);
            this->c = c;
        }
        CompoundTask getTask() {return this->c;}
};

class BlockMethods {
    private:
        vector<Method> ms;
    
    public:
        BlockMethods(Block b, BlockPrim bp, BlockComp bc, vector<CompInsertion> compIns) {
            vector<Task> tasks = {bp.getTask()};
            for (CompInsertion &ci : compIns) {
                tasks.push_back(ci.getTask());
            }
            TaskNetwork tn(tasks, true);
            string name = "insertion" + b.toString();
            Method m(name, bc.getTask(), tn);
            this->ms.push_back(m);
        }
        Method getMethod() {return this->ms[0];}
};