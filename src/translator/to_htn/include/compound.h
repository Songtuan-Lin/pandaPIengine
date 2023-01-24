#ifndef _compound_inc_h_
#define _compound_inc_h_

#include "Model.h"
#include "task.h"
#include "method.h"
#include "match.h"
#include "block.h"
#include "optimize.h"

class CompsTranslation {
    private:
        vector<CompoundTask> comps;
        int offset;
    
    public:
        CompsTranslation(Model *htn, int startID, OptimizeHTN *opt) {
            int id = startID;
            this->offset = htn->numActions;
            this->comps.resize(htn->numTasks - this->offset);
            for (int c = this->offset; c < htn->numTasks; c++) {
                if (opt->isTaskInvalid(c)) continue;
                string name = "compound[" + to_string(c) + "]";
                CompoundTask comp(name, id);
                id++;
                this->comps.push_back(comp);
            }
        }
        CompoundTask get(int c) {
            CompoundTask comp = this->comps[c - this->offset];
            assert(comp.validate());
            return comp;
        }
        vector<CompoundTask> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->comps;
        }
        bool validate() {
            for (CompoundTask &c : this->comps)
                if (!c.validate()) return false;
            return true;
        }
};

class MethodTranslation {
    private:
        Method method;
    
    public:
        MethodTranslation(
                Model *htn, int m,
                PrimsTranslation &primsTranslation, 
                CompsTranslation &compsTranslation,
                vector<BlockTranslation> &blockTranslations,
                PrimForStartingMethod &primForStartingMethod) {
            assert(blockTranslations.size() == htn->numSubTasks[m] + 1);
            string name = "method[" + to_string(m) + "]";
            vector<Task> tasks;
            tasks.resize(2 * htn->numSubTasks[m] + 1);
            for (int pos = 0; pos < tasks.size(); pos++) {
                int index = std::floor(pos / 2);
                Task t;
                if (pos % 2 == 0) {
                    t = blockTranslations[index].compForBlock.get();
                } else {
                    int subTask = htn->subTasks[m][index];
                    if (htn->isPrimitive[subTask]) {
                        t = primsTranslation.get(subTask);
                    } else {t = compsTranslation.get(subTask);}
                }
                tasks[pos] = t;
            }
            tasks.insert(tasks.begin(), primForStartingMethod.get());
            TaskNetwork tn(tasks, true);
            int decomposedTask = htn->decomposedTask[m];
            this->method = Method(name, compsTranslation.get(decomposedTask), tn);
        }
        Method get() {
            assert(this->method.validate());
            return this->method;
        }
};

#endif