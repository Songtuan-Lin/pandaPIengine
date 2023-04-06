#include "sat_verifier.h"
#include "state_formula.h"

bool SATVerifier::generateSATFormula(int depth, PDT *pdt) {
    string prefix;
    std::clock_t before, after;
    double elapsed;
    long numVariables, numVarsBefore, numVarsAfter;
    long numClauses, numClausesBefore, numClausesAfter;

    prefix = "[Generating the PDT] ";
    cout << prefix << "Depth: " << to_string(depth) << endl;
    before = std::clock();
    pdt->expandPDTUpToLevel(depth, this->htn, false);
    after = std::clock();
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;
    pdt->resetPruning(this->htn);

    prefix = "[Extracting the SOG] ";
    before = std::clock();
    vector<PDT*> leafs;
    pdt->getLeafs(leafs);
    SOG *sog = pdt->getLeafSOG();
    after = std::clock();
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    int numVertices = sog->numberOfVertices;
    cout << prefix << "Num vertices: " << to_string(numVertices) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;
    if (numVertices < this->plan.size()) return false;

    prefix = "[Generating variables for the PDT] ";
    numVarsBefore = capsule.number_of_variables;
    before = std::clock();
    pdt->assignVariableIDs(this->capsule, this->htn);
    after = std::clock();
    numVarsAfter = capsule.number_of_variables;
    numVariables = numVarsAfter - numVarsBefore;
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    cout << prefix << "Num of variables: " << to_string(numVariables) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;

    prefix = "[Generating clauses for decomposition] ";
    numClausesBefore = get_number_of_clauses();
    before = std::clock();
    pdt->addDecompositionClauses(this->solver, this->capsule, this->htn);
    no_abstract_in_leaf(this->solver, leafs, this->htn);
    after = std::clock();
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    numClausesAfter = get_number_of_clauses();
    numClauses = numClausesAfter - numClausesBefore;
    cout << prefix << "Num clauses: " << to_string(numClauses) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;

    prefix = "[Generating state variables] ";
    numVarsBefore = this->capsule.number_of_variables;
    before = std::clock();
    StateVariables *stateVars = new StateVariables(this->plan,
                                                   this->htn,
                                                   this->capsule);
    after = std::clock();
    numVarsAfter = this->capsule.number_of_variables;
    numVariables = numVarsAfter - numVarsBefore;
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    cout << prefix << "Num variables: " << to_string(numVariables) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;

    prefix = "[Generating mapping variables] ";
    numVarsBefore = this->capsule.number_of_variables;
    before = std::clock();
    PlanToSOGVars *mapping = new PlanToSOGVars(this->plan,
                                               sog,
                                               this->htn,
                                               this->capsule);
    after = std::clock();
    numVarsAfter = this->capsule.number_of_variables;
    numVariables = numVarsAfter - numVarsBefore;
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    cout << prefix << "Num variables: " << to_string(numVariables) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;

    prefix = "[Generating constraints on states] ";
    numClausesBefore = get_number_of_clauses();
    before = std::clock();
    ConstraintsOnStates(this->solver,
                        this->htn,
                        stateVars,
                        this->execution);
    after = std::clock();
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    numClausesAfter = get_number_of_clauses();
    numClauses = numClausesAfter - numClausesBefore;
    cout << prefix << "Num clauses: " << to_string(numClauses) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;

    prefix = "[Generating constraints on mapping] ";
    numClausesBefore = get_number_of_clauses();
    before = std::clock();
    ConstraintsOnMapping(this->solver,
                         this->capsule,
                         this->plan,
                         mapping,
                         sog);
    after = std::clock();
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    numClausesAfter = get_number_of_clauses();
    numClauses = numClausesAfter - numClausesBefore;
    cout << prefix << "Num clauses: " << to_string(numClauses) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;

    prefix = "[Generating clauses for decomposition] ";
    numClausesBefore = get_number_of_clauses();
    before = std::clock();
    ConstraintsOnSequence(this->solver,
                          this->htn,
                          this->plan.size(),
                          sog,
                          stateVars,
                          mapping);
    after = std::clock();
    elapsed = 1000.0 * (after - before) / CLOCKS_PER_SEC;
    numClausesAfter = get_number_of_clauses();
    numClauses = numClausesAfter - numClausesBefore;
    cout << prefix << "Num clauses: " << to_string(numClauses) << endl;
    cout << prefix << "Done! Time: " << to_string(elapsed) << endl;
    return true;
}