#include "Model.h"
#include "task.h"
#include "method.h"
#include "assert.h"

struct Slot {
    int m, b, i;
    Slot() {this->m = -1; this->b = -1; this->i = -1;}
    Slot(int m, int b, int i) {this->m = m; this->b = b; this->i = i;}
    std::string toString() {
        return "[" + std::to_string(m) + ";" + std::to_string(b) + ";" + std::to_string(i) + "]";
    }
};

struct Block {
    int m, b;
    Block() {this->m = -1; this->b = -1;}
    Block(int m, int b) {this->m = m; this->b = b;}
    std::string toString() {
        return "[" + std::to_string(m) + ";" + std::to_string(b) + "]";
    }
};