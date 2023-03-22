#include "fuzzer.h"

Fuzzer::Fuzzer(string htnFile, 
               string planFile,
               double probability) : Translator(htnFile, planFile) {
    this->model = new HTN();
    for (int p = 0; p < this->htn->numStateBits; p++) {
        string name = this->htn->factStrs[p];
        Proposition prop(name, p);
        this->model->addProps(prop);
        assert(p == this->props.size());
        this->props.push_back(prop);
    }
    for (int t = 0; t < this->htn->numTasks; t++) {
        string name = this->htn->taskNames[t];
        if (this->htn->isPrimitive[t]) {
            vector<Proposition> prec = this->convert(
                    this->htn->precLists[t], 
                    this->htn->numPrecs[t]);
            vector<Proposition> posEffs = this->convert(
                    this->htn->addLists[t], 
                    this->htn->numAdds[t]);
            vector<Proposition> negEffs = this->convert(
                    this->htn->delLists[t], 
                    this->htn->numDels[t]);
            PrimitiveTask prim(1, prec, posEffs, 
                                negEffs, name, t);
            this->model->addPrims(prim);
            assert(t == this->tasks.size());
            this->tasks.push_back(prim);
        } else {
            CompoundTask abs(name, t);
            this->model->addComps(abs);
            assert(t == this->tasks.size());
            this->tasks.push_back(abs);
        }
    }
    for (int m = 0; m < this->htn->numMethods; m++) {
        vector<Task> subs;
        string name = this->htn->methodNames[m];
        int c = this->htn->decomposedTask[m];
        Task decomposedTask = this->tasks[c];
        for (int i = 0; i < this->htn->numSubTasks[m]; i++) {
            int t = this->htn->subTasks[m][i];
            Task task = this->tasks[t];
            if (this->htn->isPrimitive[t]) {
                random_device rd;
                mt19937 gen(rd());
                discrete_distribution<> distribution(
                        {probability, 100-probability});
                int keepTask = distribution(gen);
                if (!keepTask && subs.size() != 0) continue;
            }
            subs.push_back(task);
        }
        TaskNetwork tn(subs, true);
        Method method(name, decomposedTask, tn);
        this->model->addMethods(method);
    }
    this->model->setTopTask(this->tasks[this->htn->initialTask]);
    for (int i = 0; i < this->htn->s0Size; i++) {
        int p = this->htn->s0List[i];
        this->model->addInitState(this->props[p]);
    }
    for (int i = 0; i < this->htn->gSize; i++) {
        int p = this->htn->gList[i];
        this->model->addGoal(this->props[p]);
    }
}