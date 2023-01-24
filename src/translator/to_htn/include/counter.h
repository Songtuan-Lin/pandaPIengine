#ifndef _counter_inc_h_
#define _counter_inc_h_

#include <vector>
#include "task.h"
#include "method.h"

class PropsForCounter {
    private:
        vector<Proposition> props;
    
    public:
        PropsForCounter() {}
        PropsForCounter(int range, int startID) {
            int id = startID;
            for (int i = 0; i < range; i++) {
                string name = "counted[" + to_string(i) + "]";
                Proposition prop(name, id);
                id++;
                this->props.push_back(prop);
            }
        }
        vector<Proposition> get() {return this->props;}
        Proposition getInit() {return this->props[0];}
        bool validate() {
            for (Proposition &prop : this->props)
                if (!prop.validate()) return false;
            return true;
        }
};

class PrimsForCounter {
    private:
        vector<PrimitiveTask> prims;
    
    public:
        PrimsForCounter() {}
        PrimsForCounter(PropsForCounter &propsForCounter, int startID) {
            int id = startID;
            vector<Proposition> props = propsForCounter.get();
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
        vector<PrimitiveTask> get() {return this->prims;}
        bool validate() {
            for (PrimitiveTask &p : this->prims)
                if (!p.validate()) return false;
            return true;
        }
};

class CompForCounter {
    private:
        CompoundTask c;
    
    public:
        CompForCounter() {}
        CompForCounter(int id, int counterID = -1) {
            string name = "counter[" + to_string(counterID) + "]";
            CompoundTask c(name, id);
            this->c = c;
            assert(this->c.validate());
        }
        CompoundTask get() {return this->c;}
        bool validate() {return this->c.validate();}
};

class MethodsForCounter {
    private:
        vector<Method> methods;
    
    public:
        MethodsForCounter() {}
        MethodsForCounter(
                CompForCounter compForCounter, 
                PrimsForCounter primsForCounter) {
            int i = 1;
            for (PrimitiveTask &prim : primsForCounter.get()) {
                string name = "count[" + to_string(i - 1) + ";" + to_string(i) + "]";
                vector<Task> tasks = {prim};
                TaskNetwork tn(tasks);
                Method m(name, compForCounter.get(), tn);
                methods.push_back(m);
                i++;
            }
        }
        vector<Method> get() {return this->methods;}
        bool validate() {
            for (Method &m : this->methods)
                if (!m.validate()) return false;
            return true;
        }
};

struct Counter {
    CompForCounter compForCounter;
    MethodsForCounter methodsForCounter;
    PropsForCounter propsForCounter;
    PrimsForCounter primsForCounter;

};

#endif