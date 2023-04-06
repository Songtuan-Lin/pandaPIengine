#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include "cmdline.h"
#include "to_verifier.h"
#include "verifier.h"
#include "sat_verifier.h"

int main(int argc, char *argv[]) {
    gengetopt_args_info args_info;
	if (cmdline_parser(argc, argv, &args_info) != 0) return 1;

    string htnFile = args_info.htn_arg;
    string planFile = args_info.plan_arg;
    string approach = args_info.verifier_arg;
    Verifier *verifier;
    std::clock_t beforeVerify = std::clock();
    if (approach.compare("cyk") == 0) {
        verifier = new TOVerifier(htnFile, planFile);
    } else if (approach.compare("sat") == 0) {
        verifier = new SATVerifier(htnFile, planFile);
    } else {cout << "Approach has not been implemented!" << endl; exit(-1);}
    // TODO: add the processor for selecting different verifier
    std::clock_t afterVerify = std::clock();
    double prepTime = 1000.0 * (afterVerify - beforeVerify) / CLOCKS_PER_SEC;
    cout << "Information about the verification" << endl;
    cout << "- Time for verifying the plan: " << prepTime << endl;
    bool result = verifier->getResult();

    if (result) {
        cout << "The given plan is a solution" << endl;
    }
    else {
        cout << "The given plan is not a solution" << endl;
    }

    return 0;
}