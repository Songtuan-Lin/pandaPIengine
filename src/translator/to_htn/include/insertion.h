#include "Model.h"
#include "traversal.h"

class ActionInsertions {
    private:
        Model *htn;
        vector<int> plan;
        vector<vector<vector<bool>>> valid;
    
    public:
        ActionInsertions(Model *htn, vector<int> plan) {
            this->htn = htn;
            this->plan = plan;
        }
        
        void validate() {
            this->valid.resize(this->htn->numMethods);
            for (int m = 0; m < this->htn->numMethods; m++) {
                this->valid[m].resize(this->htn->numSubTasks[m] + 1);
                for (int b = 0; b < this->htn->numSubTasks[m] + 1; b++) {
                    this->valid[m][b].resize(this->plan.size());
                    for (int s = 0; s < this->plan.size(); s++) {
                        this->valid[m][b][s] = this->validate(m, b, s);
                    }
                }
            }
        }

        bool validate(int m, int b, int i);
};