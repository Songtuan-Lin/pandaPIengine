#include "Model.h"
#include "execution.h"
#include "sat_encoder.h"

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
        int get(int pos, int prop) {return this->vars[pos][props];}
        vector<int>::iterator begin(int pos) {return this->vars[pos].begin();}
        vector<int>::iterator end(int pos) {return this->vars[pos].end();}
    private:
        vector<vector<int>> vars;
};