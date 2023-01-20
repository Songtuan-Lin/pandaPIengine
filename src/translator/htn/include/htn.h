#ifndef _htn_inc_h_
#define _htn_inc_h_

#include "task.h"
#include "method.h"

class HTN {
    private:
        vector<Proposition> props;
        vector<PrimitiveTask> prims;
        vector<CompoundTask> comps;
        vector<Method> methods;

    public:
        void addProp(Proposition prop) {this->props.push_back(prop);}
        void addPrim(PrimitiveTask prim) {this->prims.push_back(prim);}
        void addComp(CompoundTask comp) {this->comps.push_back(comp);}
        void addMethod(Method m) {this->methods.push_back(m);}
        int getNumProps() {return this->props.size();}
        int getNumPrims() {return this->prims.size();}
        int getNumComps() {return this->comps.size();}
        int getNumMethods() {return this->methods.size();}
};

#endif