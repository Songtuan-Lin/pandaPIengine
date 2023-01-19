#include "task.h"
#include "accumulation.h"

class MatchingProps {
    private:
        Proposition init;
        vector<Proposition> props;
    
    public:
        MatchingProps(int length, int startID) {
            int id = startID;
            string name = "init";
            Proposition init(name, id);
            this->init = init;
            id++;
            for (int i = 0; i < length; i++) {
                name = "matching[" + to_string(i + 1) + "]";
                Proposition prop(name, id);
                id++;
                this->props.push_back(prop);
            }
        }
        Proposition getInit() {return this->init;}
        Proposition getProp(int pos) {return this->props[pos];}
        vector<Proposition> getMatchingProps() {return this->props;}
};

class MatchingPrims {
    private:
        vector<PrimitiveTask> prims;
        vector<vector<PrimitiveTask>> lookup;
    
    public:
        MatchingPrims(
                Model *htn, int startID,
                MatchingProps &props,
                ActionPositions &positions);
        vector<PrimitiveTask> getPrims() {return this->prims;}
        vector<PrimitiveTask> getPrimsPerAction(int a) {return this->lookup[a];} 
};

class MatchingComps {
    private:
        vector<CompoundTask> comps;
        vector<CompoundTask> lookup;
    
    public:
        MatchingComps(Model *htn, int startID, ActionAccumulation &accumulation) {
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
};