#ifndef _compound_inc_h_
#define _compound_inc_h_

#include "Model.h"
#include "task.h"
#include "optimize.h"

class CompTranslation {
    private:
        vector<CompoundTask> comps;
        int offset;
    
    public:
        CompTranslation(Model *htn, int startID, OptimizeHTN *opt) {
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
        vector<CompoundTask> get() {return this->comps;}
};

#endif