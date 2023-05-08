#ifndef _VERIFIER_H_
#define _VERIFIER_H_
#include "Model.h"
#include "util.h"
#include <fstream>
#include <string.h>
#include <unordered_map>
#include <algorithm>
#include <random>

class Verifier {
    public:
        Verifier(string htnFile, string planFile) {
            this->readHTNFile(htnFile);
            vector<string> planStr = this->readPlanFile(planFile);
            this->plan = this->parsePlan(planStr);
            this->result = false;
        }

        void generateInvalPlans(
                string domain,
                string task,
                string outFile,
                string planFile) {
            random_device dev;
            mt19937 rng(dev());
            uniform_int_distribution<int> randOps(1, 3);
            uniform_int_distribution<int> position(
                    0, this->plan.size() - 1);
            uniform_int_distribution<int> action(
                    0, this->htn->numActions-1);
            int op = randOps(rng);
            int pos;
            int a;
            switch (op) {
                case 1:
                    // adding an action
                    cout << "- Adding an action" << endl;
                    pos = position(rng);
                    a = action(rng);
                    while (Util::isPrecondition(this->htn->taskNames[a]))
                        a = action(rng);
                    this->plan.insert(
                            this->plan.begin()+pos, a);
                    break;
                case 2:
                    // deleting an action
                    cout << "- Deleting an action" << endl;
                    pos = position(rng);
                    this->plan.erase(this->plan.begin() + pos);
                    break;
                case 3:
                    // switching two actions
                    cout << "- Swapping two actions" << endl;
                    int firstPos = position(rng);
                    int secondPos = position(rng);
                    swap(this->plan[firstPos], this->plan[secondPos]);
                    break;
            }
            ofstream ofile(outFile);
            ofile << domain << endl;
            ofile << task << endl;
            for (int i = 0; i < this->plan.size(); i++) {
                int t = this->plan[i];
                if (i == this->plan.size() - 1) {
                    ofile << this->htn->taskNames[t];
                } else { ofile << this->htn->taskNames[t] << ";"; }
            }
            ofile.close();
            ofstream oplan(planFile);
            for (int i = 0; i < this->plan.size(); i++) {
                int t = this->plan[i];
                if (i == this->plan.size() - 1) {
                    oplan << this->htn->taskNames[t];
                } else {oplan << this->htn->taskNames[t] << ";";}
            }
        };
        
        virtual bool getResult() {return this->result;}

    protected:
        Model *htn;
        vector<int> plan;
        bool result;

    private:
        void readHTNFile(string htnFile) {
            cout << "read model from" << htnFile << "\n";
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
            cout << "Converting plan" << endl;
            vector<int> planNum;
            unordered_map<string, int> taskToIndex;
            for (int i = 0; i < this->htn->numTasks; i++) {
                string taskName = this->htn->taskNames[i];
                std::transform(taskName.begin(), taskName.end(), taskName.begin(), ::tolower);
                taskToIndex.insert({taskName, i});
            }
            for (int i = 0; i < planStr.size(); i++) {
                if (!taskToIndex.count(planStr[i])) {
                    std::cerr << "Plan contains unreachable actions, plan is not a solution" << endl;
                    exit(-1);
                }
                planNum.push_back(taskToIndex[planStr[i]]);
            }
            cout << "Converting plan done" << endl;
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
};
#endif