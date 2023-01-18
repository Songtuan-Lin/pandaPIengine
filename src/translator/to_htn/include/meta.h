#include "Model.h"
#include "task.h"
#include "method.h"
#include "assert.h"

class MetaInfo {
    private:
        vector<PrimitiveTask> prims;
        vector<CompoundTask> comps;
        vector<Proposition> props;
        vector<Method> methods;
    
    public:
        void addPrimitiveTask(PrimitiveTask p) {
            assert(p.validiate());
            this->prims.push_back(p);
        }
        void addCompoundTask(CompoundTask c) {
            assert(c.validiate());
            this->comps.push_back(c);
        }
        void addProposition(Proposition prop) {
            assert(prop.validiate());
            this->props.push_back(prop);
        }
        void addMethod(Method m) {this->methods.push_back(m);}
        int getNumPrimitive() {return this->prims.size();}
        int getNumCompound() {return this->comps.size();}
        int getNumProps() {return this->props.size();}
        int getNumMethods() {return this->methods.size();}
        vector<PrimitiveTask>::iterator primBegin() {return this->prims.begin();}
        vector<PrimitiveTask>::iterator primEnd() {return this->prims.end();}
        vector<CompoundTask>::iterator compBegin() {return this->comps.begin();}
        vector<CompoundTask>::iterator compEnd() {return this->comps.end();}
        vector<Proposition>::iterator propBegin() {return this->props.begin();}
        vector<Proposition>::iterator propEnd() {return this->props.end();}
        vector<Method>::iterator methodBegin() {return this->methods.begin();}
        vector<Method>::iterator methodEnd() {return this->methods.end();}
};