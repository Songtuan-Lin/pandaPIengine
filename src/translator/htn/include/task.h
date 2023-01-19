#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include "assert.h"

using namespace std;

class Proposition {
    private:
        int id;
        string name;
    
    public:
        Proposition() {
            this->id = -1;
            this->name = "";
        }
        Proposition(string name, int id = -1) {
            this->id = id;
            this->name = name;
        }
        int getID() {return this->id;}
        string getName() {return this->name;}
        bool validate() {return this->id != -1;}
        void write(ofstream &ofile) {
            assert(this->validate());
            ofile << this->name << endl;
        }
};

class Task {
    private:
        int id;
        string name;

    public:
        Task() {
            this->id = -1;
            this->name = "";
        }
        Task(string name, int id = -1) {
            this->id = id;
            this->name = name;
        }
        virtual bool isPrimitive() {return false;}
        virtual void write(ofstream &ofile) {}
        int getID() {return this->id;}
        string getName() {return this->name;}
        bool validate() {return this->id != -1;}
};

class PrimitiveTask : public Task {
    private:
        vector<Proposition> precondition;
        vector<Proposition> posEffs;
        vector<Proposition> negEffs;
        int cost;

    public:
        PrimitiveTask() : Task() {}
        PrimitiveTask(string name, int id = -1) : Task(name, id) {}
        PrimitiveTask(int cost, vector<Proposition> precondition, vector<Proposition> posEffs, vector<Proposition> negEffs, string name, int id = -1) : Task(name, id) {
            this->cost = cost;
            this->precondition = precondition;
            this->posEffs = posEffs;
            this->negEffs = negEffs;
        }
        bool isPrimitive() {return true;}
        void write(ofstream &ofile) {
            assert(this->validate());
            ofile << "0 " << this->getName() << endl;
        }
        void writeAsAction(ofstream &ofile) {
            ofile << to_string(this->cost) << endl;
            for (Proposition p : this->precondition) {
                ofile << p.getID() << " ";
            }
            ofile << -1 << endl;
            for (Proposition p : this->posEffs) {
                ofile << "0 " << p.getID() << "  ";
            }
            ofile << -1 << endl;
            for (Proposition p : this->negEffs) {
                ofile << "0 " << p.getID() << "  ";
            }
            ofile << -1 << endl;
        }
};

class CompoundTask : public Task {
    public:
        CompoundTask() : Task() {}
        CompoundTask(string name, int id = -1) : Task(name, id) {}
        bool isPrimitive() {return false;}
        void write(ofstream &ofile) {
            assert(this->validate());
            ofile << "1 " << this->getName() << endl;
        }
};

class TaskNetwork {
    private:
        vector<Task> tasks; // the index of each task is viewed as the identifier 
        vector<tuple<int, int>> order;

    public:
        TaskNetwork() {}
        TaskNetwork(vector<Task> tasks, bool ordered = false) {
            this->tasks = tasks;
            if (ordered && tasks.size() > 1)
                for (int i = 1; i < tasks.size(); i++)
                    this->order.push_back(make_tuple(i - 1, i));
        }
        TaskNetwork(vector<Task> tasks, vector<tuple<int, int>> order) {
            this->tasks = tasks;
            this->order = order;
        }
        void addTask(Task task) {this->tasks.push_back(task);}
        void addOrder(tuple<int, int> constraint) {
            assert(get<0>(constraint) < this->tasks.size() && get<1>(constraint) < this->tasks.size());
            this->order.push_back(constraint);
        }
        bool validiate() {
            for (Task t: this->tasks) {
                if (!t.validate()) return false;
            }
            return true;
        }
        void write(ofstream &ofile) {
            assert(this->validiate());
            for (Task t : this->tasks) {
                // TODO: check carefully whether an offset should
                // be added to the id of a compound task
                ofile << t.getID() << " ";
            }
            ofile << -1 << endl;
            for (tuple<int, int> o: this->order) {
                ofile << get<0>(o) << " " << get<1>(o) << " ";
            }
            ofile << -1 << endl;
        }
};