//
// Created by u6162630 on 4/17/23.
//

#ifndef PANDAPIENGINE_DISTRIBUTION_H
#define PANDAPIENGINE_DISTRIBUTION_H

#include <climits>
#include "vector"

using namespace std;
class LengthDistributions {
public:
    LengthDistributions(int total, int numCols) {
        this->numCols = numCols;
        this->distribution = new int*[INT_MAX];
        this->numRows = this->generate(total, 0, 0);
        this->row = 0;
    }
    int* next() {
        if (this->row >= this->numRows)
            return nullptr;
        int r = this->row;
        this->row++;
        return this->distribution[r];
    }
    void reset() {this->row = 0;}
    int numDistributions() {return this->numRows;}
private:
    int** distribution;
    int row;
    int numCols;
    int numRows;
    int generate(int total, int row, int col);
};

class Distributions {
public:
    Distributions(int method, int length, Model *htn) {
        int numTasks = htn->numSubTasks[method];
        this->valid.resize(numTasks);
        for (int i = 0; i < numTasks; i++)
            this->valid[i].assign(length + 1, false);
        this->depth.resize(numTasks);
        for (int i = 0; i < numTasks; i++)
            this->depth[i].assign(length + 1, -1);
        this->method = method;
        this->maxLength = length;
        this->htn = htn;
    }
    void update(int length, vector<vector<int>> &K, bool allowEmptiness) {
        int numTasks = this->htn->numSubTasks[this->method];
        int rootTask = this->htn->decomposedTask[this->method];
        int lastTask = this->htn->subTasks[this->method][numTasks - 1];
        int subtaskSCC = this->htn->taskToSCC[lastTask];
        int rootSCC = this->htn->taskToSCC[rootTask];
        if ((subtaskSCC == rootSCC) == allowEmptiness) {
            this->valid[numTasks - 1][length] = (K[lastTask][length] != -1);
            this->depth[numTasks - 1][length] = max(
                    this->depth[numTasks - 1][length],
                    K[lastTask][length]);
        }
        // TODO: update the depth
        for (int tInd = numTasks - 2; tInd >= 0; tInd--) {
            int task = this->htn->subTasks[this->method][tInd];
            subtaskSCC = this->htn->taskToSCC[task];
            if ((subtaskSCC == rootSCC) == allowEmptiness) {
                this->valid[tInd][length] = this->valid[tInd][length] ||
                                            this->valid[tInd + 1][0];
                this->depth[tInd][length] = max(
                        this->depth[tInd][length],
                        K[task][length]);
            }
            if (allowEmptiness) continue;
            for (int assignment = 0; assignment < length; assignment++) {
                int depthForTask = K[task][assignment];
                if (depthForTask == -1) continue;
                int remain = length - assignment;
                if (this->valid[tInd + 1][remain]) {
                    this->valid[tInd][length] = true;
                    //TODO: update the depth
                    int depthForSubtasks = this->depth[tInd + 1][remain];
                    this->depth[tInd][length] = max(
                            this->depth[tInd][length],
                            depthForTask + depthForSubtasks);
                }
            }
        }
    }
    bool distributable(int tInd, int length);
private:
    Model *htn;
    int method;
    int maxLength;
    vector<vector<bool>> valid;
    vector<vector<int>> depth;
};
#endif //PANDAPIENGINE_DISTRIBUTION_H
