#ifndef _htn_inc_h_
#define _htn_inc_h_

#include "task.h"
#include "method.h"

class HTN {
    private:
        vector<Proposition> props;
        vector<PrimitiveTask> prims;
        vector<CompoundTask> comps;
        vector<Method> methods;
        vector<Proposition> init;
        vector<Proposition> goal;
        CompoundTask top;

    public:
        void setTopTask(CompoundTask c) {this->top = c; assert(this->top.validate());}
        void addProps(Proposition prop) {this->props.push_back(prop);}
        void addProps(vector<Proposition> props) {
            this->props.insert(this->props.end(), props.begin(), props.end());
        }
        void addPrims(PrimitiveTask prim) {this->prims.push_back(prim);}
        void addPrims(vector<PrimitiveTask> prims) {
            this->prims.insert(this->prims.end(), prims.begin(), prims.end());
        }
        void addComps(CompoundTask comp) {this->comps.push_back(comp);}
        void addComps(vector<CompoundTask> comps) {
            this->comps.insert(this->comps.end(), comps.begin(), comps.end());
        }
        void addMethods(Method m) {this->methods.push_back(m);}
        void addMethods(vector<Method> ms) {
            this->methods.insert(this->methods.end(), ms.begin(), ms.end());
        }
        void addInitState(Proposition prop) {this->init.push_back(prop);}
        void addGoal(Proposition prop) {this->goal.push_back(prop);}
        int getNumProps() {return this->props.size();}
        int getNumPrims() {return this->prims.size();}
        int getNumComps() {return this->comps.size();}
        int getNumMethods() {return this->methods.size();}
        void write(ofstream &ofile) {
            // write propositions
            ofile << ";; #state features" << endl;
            ofile << this->props.size() << endl;
            for (Proposition prop : this->props)
                prop.write(ofile);
            // write mutex
            ofile << endl << ";; Mutex Groups" << endl;
            ofile << this->props.size() << endl;
            for (Proposition prop : this->props) {
                ofile << prop.getID() << " "; 
                ofile << prop.getID() << " ";
                ofile << prop.getName() << endl;
            }
            // other mutex groups
            ofile << endl << ";; further strict Mutex Groups" << endl;
            ofile << 0 << endl;
            ofile << endl << ";; further non strict Mutex Groups" << endl;
            ofile << 0 << endl;
            // invariants
            ofile << endl << ";; known invariants" << endl;
            ofile << 0 << endl;
            // actions
            ofile << endl << ";; Actions" << endl;
            ofile << this->prims.size() << endl;
            for (PrimitiveTask &prim : this->prims)
                prim.writeAsAction(ofile);
            // initial state
            ofile << endl << ";; Initial State" << endl;
            for (Proposition &prop : this->init)
                ofile << prop.getID() << " ";
            ofile << " -1" << endl;
            // goal
            ofile << endl << ";; Goal" << endl;
            for (Proposition &prop : this->goal) 
                ofile << prop.getID() << " ";
            ofile << " -1" << endl;
            // all tasks
            ofile << endl << ";; Tasks (primitive and abstract)" << endl;
            ofile << this->prims.size() + this->comps.size() << endl;
            for (PrimitiveTask &prim : this->prims)
                prim.write(ofile);
            for (CompoundTask &c : this->comps)
                c.write(ofile);
            // initial task
            ofile << endl << ";; Initial Abstract Task" << endl;
            ofile << this->top.getID() << endl;
            // methods
            ofile << endl << ";; Methods" << endl;
            ofile << this->methods.size() << endl;
            for (Method &m : this->methods)
                m.write(ofile);
        }
        bool validate() {
            for (int i = 0; i < this->props.size(); i++)
                if (i != this->props[i].getID()) 
                    return false;
            for (int i = 0; i < this->prims.size(); i++)
                if (i != this->prims[i].getID()) 
                    return false;
            int offset = this->prims.size();
            for (int i = 0; i < this->comps.size(); i++)
                if (i + offset != this->comps[i].getID()) 
                    return false;
            return true;
        }
};

#endif