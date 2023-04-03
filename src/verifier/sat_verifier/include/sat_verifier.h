#include "Model.h"
#include "sog.h"
#include "verifier.h"
#include "execution.h"
#include "sat_encoder.h"

class SATVerifier : Verifier {
    public:
        SATVerifier(string htnFile, string planFile) {}
    
    private:
        sat_capsule capsule;
        SOG *sog;
};