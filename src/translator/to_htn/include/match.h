#ifndef _match_inc_h_
#define _match_inc_h_

#include "task.h"
#include "method.h"
#include "accumulation.h"

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
        Proposition getInit() {return this->init;}
        Proposition getProp(int pos) {return this->props[pos];}
        vector<Proposition> getMatchingProps() {return this->props;}
        vector<Proposition> get() {
            vector<Proposition> v = {this->init};
            v.insert(v.end(), this->props.begin(), this->props.end());
            return v;
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
                ActionPositions &positions);
        vector<PrimitiveTask> get() {return this->prims;}
        vector<PrimitiveTask> get(int a) {return this->lookup[a];} 
};

class PrimsTranslation {
    private:
        vector<CompoundTask> comps;
        vector<CompoundTask> lookup;
    
    public:
        PrimsTranslation(Model *htn, int startID, ActionAccumulation &accumulation) {
            int id = startID;
            this->lookup.resize(htn->numActions);
            for (int a = 0; a < htn->numActions; a++) {
                if (accumulation.getNumAccumulation(a) == 0) continue;
                string name = "prim[" + to_string(a) + "]";
                CompoundTask c(name, id);
                this->comps.push_back(c);
                this->lookup[a] = c;
                id++;
            }
        }
        CompoundTask get(int a) {return this->lookup[a];}
        vector<CompoundTask> get() {return this->comps;}
};

class MethodsForMatching {
    private:
        vector<Method> methods;
    
    public:
        MethodsForMatching(
                Model *htn, 
                PrimsTranslation translation, 
                PrimsForMatching matching);
        vector<Method> get() {return this->methods;}
};

#endif