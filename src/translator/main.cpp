#include "translator.h"
#include "to_htn.h"
#include "istream"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Invalid Arguments" << endl;
        exit(-1);
    }
    string task = argv[1];
    string htnFile = argv[2];
    string planFile = argv[3];
    string outFile = argv[4];
    Translator *translator;
    if (task.compare("to_htn") == 0) {
        translator = new HTNTranslator(
                htnFile, planFile);
    } else if (task.compare("to_pddl") == 0) {
        cout << "Not delivered" << endl;
        exit(-1);
    } else {
        cerr << "Invalid Arguments" << endl;
        exit(-1);
    }
    ofstream ofile(outFile);
    translator->write(ofile);
}