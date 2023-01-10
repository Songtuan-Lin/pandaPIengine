#include <string>
#include <vector>
#include "assert.h"

using namespace std;
class Task {
    private:
        int id;
        string name;

    public:
        Task(int id, string name) {
            this->id = id;
            this->name = name;
        }
        virtual bool isPrimitive() {return false;}
        int getID() {return this->id;}
        string getName() {return this->name;}
};

class PrimitiveTask : public Task {
    public:
        PrimitiveTask(int id, string name) : Task(id, name) {}
        bool isPrimitive() {return true;}
};

class CompoundTask : public Task {
    public:
        CompoundTask(int id, string name) : Task(id, name) {}
        bool isPrimitive() {return false;}
};

class TaskNetwork {
    private:
        vector<Task> tasks; // the index of each task is viewed as the identifier 
        vector<tuple<int, int>> order;

    public:
        void addTask(Task task) {this->tasks.push_back(task);}
        void addOrder(tuple<int, int> constraint) {
            assert(get<0>(constraint) < this->tasks.size() && get<1>(constraint) < this->tasks.size());
            this->order.push_back(constraint);
        }
};