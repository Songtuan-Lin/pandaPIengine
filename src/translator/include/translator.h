#include "Model.h"
#include "task.h"
#include "method.h"
#include "traversal.h"
#include "optimize.h"
#include "validation.h"
#include <string.h>
#include <unordered_map>
#include <algorithm>

class Translator {
    protected:
        Model *htn;
        vector<int> plan;
        TaskTraversal *traversal;
        OptimizeHTN *optimizeHTN;
        SlotValidation *validation;
    
    private:
        void readHTNFile(string htnFile) {
            cout << "Read model from" << htnFile << "\n";
            std::ifstream *fileInput = new std::ifstream(htnFile);
            if(!fileInput->good()) {
                std::cerr << "Unable to open input file " << htnFile << ": " << strerror (errno) << std::endl;
                exit(-1);
            }
            std::istream * inputStream;
            inputStream = fileInput;

            bool useTaskHash = true;
            /* Read model */
            // todo: the correct value of maintainTaskRechability depends on the heuristic
            eMaintainTaskReachability reachability = mtrALL;
            bool trackContainedTasks = useTaskHash;
            this->htn = new Model(trackContainedTasks, reachability, true, true);
            this->htn->filename = htnFile;
            this->htn->read(inputStream);
            cout << "reading htn file completed" << endl;
        }
        vector<int> parsePlan(vector<string> planStr) {
            vector<int> planNum;
            unordered_map<string, int> taskToIndex;
            for (int i = 0; i < this->htn->numTasks; i++) {
                string taskName = this->htn->taskNames[i];
                std::transform(taskName.begin(), taskName.end(), taskName.begin(), ::tolower);
                taskToIndex.insert({taskName, i});
            }
            for (int i = 0; i < planStr.size(); i++) {
                if (!taskToIndex.count(planStr[i])) {
                    std::cerr << "Error: Plan contains actions not in the domain" << endl;
                    exit(-1);
                }
                planNum.push_back(taskToIndex[planStr[i]]);
            }
            return planNum;
        }
        vector<string> readPlanFile(string planFile) {
            cout << "Read plan from " << planFile << endl;
            vector<string> plan;
            ifstream fin(planFile);
            if(!fin.good()) {
                std::cerr << "Unable to open input file " << planFile << ": " << strerror (errno) << std::endl;
                exit(-1);
            }
            string action;
            while(std::getline(fin, action, ';')) {
                action.erase(std::remove(action.begin(), action.end(), '\n'), action.end());
                std::transform(action.begin(), action.end(), action.begin(), ::tolower);
                plan.push_back(action);
            }
            cout << "Read plan file complete" << endl;
            return plan;
        }

    public:
        Translator(string htnFile, string planFile) {
            this->readHTNFile(htnFile);
            vector<string> planStr = this->readPlanFile(planFile);
            this->plan = this->parsePlan(planStr);
            this->traversal = new TaskTraversal(this->htn);
            this->validation = new SlotValidation(this->htn, this->plan);
            this->optimizeHTN = new OptimizeHTN(this->htn, this->plan);
        }
};