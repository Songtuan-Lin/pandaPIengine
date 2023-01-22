#ifndef _counter_inc_h_
#define _counter_inc_h_

#include <vector>
#include "task.h"
#include "method.h"

class PropsForCounters {
    private:
        vector<Proposition> props;
    
    public:
        PropsForCounters() {}
        PropsForCounters(int range, int startID) {
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
};

class PrimsForCounters {
    private:
        vector<PrimitiveTask> prims;
    
    public:
        PrimsForCounters() {}
        PrimsForCounters(PropsForCounters &propsForCounter, int startID) {
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
};

class CompsForCounters {
    private:
        CompoundTask c;
    
    public:
        CompsForCounters() {}
        CompsForCounters(int id, int counterID = -1) {
            string name = "counter[" + to_string(counterID) + "]";
            CompoundTask c(name, id);
            this->c = c;
            assert(this->c.validate());
        }
        CompoundTask getComp() {return this->c;}
};

class MethodsForCounters {
    private:
        vector<Method> methods;
    
    public:
        MethodsForCounters() {}
        MethodsForCounters(
                CompsForCounters compForCounter, 
                PrimsForCounters primsForCounter) {
            int i = 1;
            for (PrimitiveTask &prim : primsForCounter.get()) {
                string name = "count[" + to_string(i - 1) + ";" + to_string(i) + "]";
                vector<Task> tasks = {prim};
                TaskNetwork tn(tasks);
                Method m(name, compForCounter.getComp(), tn);
                methods.push_back(m);
                i++;
            }
        }
        vector<Method> getMethods() {return this->methods;}
};

struct Counter {
    CompsForCounters c;
    MethodsForCounters ms;
    PropsForCounters props;
    PrimsForCounters prims;
    
    Counter(
            CompsForCounters c, 
            MethodsForCounters ms, 
            PropsForCounters props, 
            PrimsForCounters prims) {
        this->c = c;
        this->ms = ms;
        this->props = props;
        this->prims = prims;
    }
};

#endif