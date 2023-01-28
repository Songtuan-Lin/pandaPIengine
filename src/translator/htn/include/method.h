#ifndef _method_inc_h_
#define _method_inc_h_

#include "task.h"

class Method {
    private:
        CompoundTask t; // decomposed task
        TaskNetwork tn; // task network
        string name; // method name

    public:
        Method() {};
        Method(string name, CompoundTask t, TaskNetwork tn) {
            this->name = name;
            this->t = t;
            this->tn = tn;
        }
        void write(ofstream &ofile) {
            ofile << this->name << endl;
            // TODO: carefully check whether an offset 
            // should be added
            ofile << this->t.getID() << endl;
            this->tn.write(ofile);
        }
        bool validate() {return this->t.validate();}
        CompoundTask getDecomposedTask() {return this->t;}
        TaskNetwork getTaskNetwork() {return this->tn;}
};

#endif