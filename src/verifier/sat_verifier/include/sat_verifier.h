#include "Model.h"
#include "sog.h"
#include "pdt.h"
#include "verifier.h"
#include "execution.h"
#include "variables.h"
#include "constraints.h"
#include "sat_encoder.h"

class SATVerifier : Verifier {
    public:
        SATVerifier(string htnFile, string planFile) : Verifier(htnFile, planFile) {}
    
    private:
        sat_capsule capsule;
        PlanExecution *execution;
        void *solver;

        bool generateSATFormula(int depth, PDT *pdt);
};