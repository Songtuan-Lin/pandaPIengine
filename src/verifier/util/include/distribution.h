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
#endif //PANDAPIENGINE_DISTRIBUTION_H
