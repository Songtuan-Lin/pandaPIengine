#include "Model.h"
#include "execution.h"
#include "sat_encoder.h"
#include "sog.h"
#include "pdt.h"

class StateVariables {
    public:
        StateVariables(
                vector<int> &plan, 
                Model *htn, 
                sat_capsule &capsule) {
            for (int pos = 0; pos < plan.size() + 1; pos++) {
                vector<int> varsPerPos;
                int total = htn->numStateBits;
                for (int prop = 0; prop < total; prop++) {
                    int v = capsule.new_variable();
                    string propName = htn->factStrs[prop];
                    string varName = propName + "@" + to_string(pos);
                    DEBUG(capsule.registerVariable(v, varName));
                    varsPerPos.push_back(v);
                }
                this->vars.push_back(varsPerPos);
            }
        }
        int get(int pos, int prop) {return this->vars[pos][prop];}
        vector<int>::iterator begin(int pos) {return this->vars[pos].begin();}
        vector<int>::iterator end(int pos) {return this->vars[pos].end();}
    private:
        vector<vector<int>> vars;
};

class PlanToSOGVars {
public:
    PlanToSOGVars(
            vector<int> &plan,
            SOG *sog,
            Model *htn,
            sat_capsule &capsule);
    int getMatchedVar(int pos) {return this->matched[pos];}
    int getMappingVar(int pos, int v) {return this->mapping[pos][v];}
    int getForbiddenVar(int pos, int v) {return this->forbidden[pos][v];}
    int getTaskVar(int pos, int v) {return this->tasks[pos][v];}


private:
    vector<int> matched;
    vector<vector<int>> mapping;
    vector<vector<int>> forbidden;
    vector<vector<int>> tasks;
};

