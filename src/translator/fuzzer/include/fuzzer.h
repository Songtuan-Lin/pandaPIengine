#include "Model.h"
#include "htn.h"
#include "task.h"
#include "translator.h"
#include <random>

class Fuzzer : public Translator {
    public:
        Fuzzer(string htnFile, string planFile, double probability=30);
        void write(ofstream &ofile) {this->model->write(ofile);}
    private:
        HTN *model;
        vector<Proposition> props;
        vector<Task> tasks;

        vector<Proposition> convert(int *list, int len) {
            vector<Proposition> propositions;
            for (int i = 0; i < len; i++) {
                Proposition prop = this->props[list[i]];
                propositions.push_back(prop);
            }
            return propositions;
        }
};