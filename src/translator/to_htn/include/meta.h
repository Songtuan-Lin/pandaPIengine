#ifndef _meta_inc_h_
#define _meta_inc_h_

#include "assert.h"
#include "insertion.h"
#include "block.h"
#include "counter.h"

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

struct SlotTranslation {
    PropsForInsertion *propsForInsertion = nullptr;
    PrimsForInsertion *primsForInsertion = nullptr;
    CompForInsertion *compForInsertion = nullptr;
    MethodsForInsertion *methodsForInsertion = nullptr;
};

#endif