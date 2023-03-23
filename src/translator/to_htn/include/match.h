#ifndef _match_inc_h_
#define _match_inc_h_

#include "task.h"
#include "method.h"
#include "accumulation.h"
#include "optimize.h"

class PropsForMatching {
    private:
        Proposition init;
        vector<Proposition> props;
    
    public:
        PropsForMatching(int length, int startID) {
            int id = startID;
            string name = "init";
            Proposition init(name, id);
            this->init = init;
            id++;
            for (int i = 0; i < length; i++) {
                name = "matched[" + to_string(i + 1) + "]";
                Proposition prop(name, id);
                id++;
                this->props.push_back(prop);
            }
        }
        Proposition getInit() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->init;
        }
        Proposition getProp(int pos) {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->props[pos];
        }
        vector<Proposition> getMatchingProps() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->props;
        }
        vector<Proposition> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            vector<Proposition> v = {this->init};
            v.insert(v.end(), this->props.begin(), this->props.end());
            return v;
        }
        bool validate() {
            for (Proposition &prop : this->props)
                if (!prop.validate()) return false;
            return true && this->init.validate();
        }
};

class PrimsForMatching {
    private:
        vector<PrimitiveTask> prims;
        vector<vector<PrimitiveTask>> lookup;
    
    public:
        PrimsForMatching(
                Model *htn, int startID,
                PropsForMatching &props,
                ActionPositions &positions,
                OptimizeHTN *optimizeHTN);
        vector<PrimitiveTask> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->prims;
        }
        vector<PrimitiveTask> get(int a) {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->lookup[a];
        } 
        bool validate() {
            for (PrimitiveTask &p : this->prims)
                if (!p.validate()) return false;
            return true;
        }
};

class PrimsTranslation {
    private:
        vector<CompoundTask> comps;
        vector<CompoundTask> lookup;
    
    public:
        PrimsTranslation(Model *htn, 
                         int startID, 
                         ActionAccumulation &accumulation,
                         OptimizeHTN *optimizeHTN) {
            int id = startID;
            this->lookup.resize(htn->numActions);
            for (int a = 0; a < htn->numActions; a++) {
                if (optimizeHTN->isTaskInvalid(a)) continue;
                if (accumulation.getNumAccumulation(a) == 0) continue;
                string name = "prim[" + to_string(a) + "]";
                CompoundTask c(name, id);
                this->comps.push_back(c);
                this->lookup[a] = c;
                id++;
            }
        }
        CompoundTask get(int a) {
#ifndef NDEBUG
            assert(this->lookup[a].validate());
#endif
            return this->lookup[a];
        }
        vector<CompoundTask> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->comps;
        }
        bool validate() {
            for (CompoundTask &c : comps)
                if (!c.validate()) return false;
            return true;
        }
};

class MethodsForMatching {
    private:
        vector<Method> methods;
    
    public:
        MethodsForMatching(
                Model *htn, 
                PrimsTranslation &translation, 
                PrimsForMatching &primsForMatching,
                ActionAccumulation &accumulation,
                OptimizeHTN *optimizeHTN);
        vector<Method> get() {
#ifndef NDEBUG
            assert(this->validate());
#endif
            return this->methods;
        }
        bool validate() {
            for (Method &m : this->methods)
                if (!m.validate()) return false;
            return true;
        }
};

#endif