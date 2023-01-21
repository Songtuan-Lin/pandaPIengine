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
        void addProps(Proposition prop) {this->props.push_back(prop);}
        void addProps(vector<Proposition> props) {
            this->props.insert(this->props.end(), props.begin(), props.end());
        }
        void addPrims(PrimitiveTask prim) {this->prims.push_back(prim);}
        void addPrims(vector<PrimitiveTask> prims) {
            this->prims.insert(this->prims.end(), prims.begin(), prims.end());
        }
        void addComps(CompoundTask comp) {this->comps.push_back(comp);}
        void addComps(vector<CompoundTask> comps) {
            this->comps.insert(this->comps.end(), comps.begin(), comps.end());
        }
        void addMethods(Method m) {this->methods.push_back(m);}
        void addMethods(vector<Method> ms) {
            this->methods.insert(this->methods.end(), ms.begin(), ms.end());
        }
        int getNumProps() {return this->props.size();}
        int getNumPrims() {return this->prims.size();}
        int getNumComps() {return this->comps.size();}
        int getNumMethods() {return this->methods.size();}
};

#endif