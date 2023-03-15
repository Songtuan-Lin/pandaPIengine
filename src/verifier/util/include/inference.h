#include "Model.h"
#include "PrecsEffs.h"
#include <boost/dynamic_bitset.hpp>

class InferredPrecsEffs {
    private:
        int offset;
        vector<boost::dynamic_bitset<>> precsForCompoundTasks;
        vector<boost::dynamic_bitset<>> posEffsForCompoundTasks;
        vector<boost::dynamic_bitset<>> negEffsForCompoundTasks;

    public:
        InferredPrecsEffs(Model *htn) {
            this->offset = htn->numActions;
            int numCompounTasks = htn->numTasks - htn->numActions;
            computeEffectsAndPreconditions(
                    htn, htn->poss_eff_positive, 
                    htn->poss_eff_negative,
                    htn->eff_positive, 
                    htn->eff_negative,
                    htn->preconditions,
                    numCompounTasks);
            for (int c = 0; c < numCompounTasks; c++) {
                boost::dynamic_bitset<> precs(htn->numStateBits);
                int bound = htn->preconditions[c].size();
                for (int i = 0; i < bound; i++)
                    precs[htn->preconditions[c][i]] = true;
                precsForCompoundTasks.push_back(precs);
                boost::dynamic_bitset<> posEffs(htn->numStateBits);
                bound = htn->eff_positive[c].size();
                for (int i = 0; i < bound; i++)
                    posEffs[htn->eff_positive[c][i]] = true;
                posEffsForCompoundTasks.push_back(posEffs);
                boost::dynamic_bitset<> negEffs(htn->numStateBits);
                bound = htn->eff_negative[c].size();
                for (int i = 0; i < bound; i++)
                    negEffs[htn->eff_negative[c][i]] = true;
                negEffsForCompoundTasks.push_back(negEffs);
            }
        }
        boost::dynamic_bitset<> getPrecs(int c) {
            assert(c - this->offset >= 0);
            return precsForCompoundTasks[c - this->offset];
        }
        boost::dynamic_bitset<> getPosEffs(int c) {
            assert(c - this->offset >= 0);
            return posEffsForCompoundTasks[c - this->offset];
        }
        boost::dynamic_bitset<> getNegEffs(int c) {
            assert(c - this->offset >= 0);
            return negEffsForCompoundTasks[c - this->offset];
        }
};