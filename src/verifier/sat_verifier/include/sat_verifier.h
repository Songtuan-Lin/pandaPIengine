#include "Model.h"
#include "sog.h"
#include "pdt.h"
#include "ipasir.h"
#include "verifier.h"
#include "execution.h"
#include "variables.h"
#include "constraints.h"
#include "sat_encoder.h"

class SATVerifier : public Verifier {
    public:
        SATVerifier(string htnFile, string planFile) : Verifier(htnFile, planFile) {
            this->execution = new PlanExecution(this->htn, this->plan);
            this->htn->computeTransitiveClosureOfMethodOrderings();
            this->htn->buildOrderingDatastructures();
            this->htn->isTotallyOrdered = false;
            PDT *pdt = new PDT(this->htn);
            int maxDepth = 2 * (this->plan.size()) * (this->htn->numTasks - this->htn->numActions);
            for (int depth = 1; depth < maxDepth; depth++) {
                this->solver = ipasir_init();
                int state = 20;
                if (this->generateSATFormula(depth, pdt)) {
                    string prefix = "[Solving the SAT formula] ";
                    std::clock_t before = std::clock();
                    state = ipasir_solve(this->solver);
                    std::clock_t after = std::clock();
                    double elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
                    if (state == 10) {
                        cout << prefix << "Solved!" << endl;
                        this->result = true;
                        ipasir_release(this->solver);
                        cout << prefix << "Time: " << to_string(elapsed) << endl;
                        return;
                    } else {cout << prefix << "Unsolvable" << endl;}
                    cout << prefix << "Time: " << to_string(elapsed) << endl;
                }
                ipasir_release(this->solver);
            }
        }
    
    private:
        sat_capsule capsule;
        PlanExecution *execution;
        void *solver;

        bool generateSATFormula(int depth, PDT *pdt);
};