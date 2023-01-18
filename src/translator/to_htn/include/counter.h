#include <vector>
#include "task.h"
#include "method.h"

class CounterProps {
    private:
        vector<Proposition> props;
    
    public:
        CounterProps(int range, int startID) {
            int id = startID;
            for (int i = 0; i < range; i++) {
                string name = "counted[" + to_string(i) + "]";
                Proposition prop(name, id);
                id++;
                this->props.push_back(prop);
            }
        }
        vector<Proposition> getProps() {return props;}
};

class CounterPrims {
    private:
        vector<PrimitiveTask> prims;
    
    public:
        CounterPrims(const vector<Proposition> &props, int startID) {
            int id = startID;
            for (int i = 1; i < props.size(); i++) {
                string name = "counting[" + to_string(i) + "]";
                vector<Proposition> prec = {props[i - 1]};
                vector<Proposition> del = {props[i - 1]};
                vector<Proposition> add = {props[i]};
                PrimitiveTask prim(0, prec, add, del, name, id);
                assert(prim.validate());
                id++;
                this->prims.push_back(prim);
            }
        }
        vector<PrimitiveTask> getPrims() {return this->prims;}
};

class CounterComp {
    private:
        CompoundTask c;
    
    public:
        CounterComp(int id, int counterID = -1) {
            string name = "counter[" + to_string(counterID) + "]";
            CompoundTask c(name, id);
            this->c = c;
            assert(this->c.validate());
        }
        CompoundTask getComp() {return this->c;}
};

class CounterMethods {
    private:
        vector<Method> methods;
    
    public:
        CounterMethods(CounterComp c, CounterPrims cps) {
            int i = 1;
            for (PrimitiveTask &prim : cps.getPrims()) {
                string name = "count[" + to_string(i - 1) + ";" + to_string(i) + "]";
                vector<Task> tasks = {prim};
                TaskNetwork tn(tasks);
                Method m(name, c.getComp(), tn);
                methods.push_back(m);
                i++;
            }
        }
};