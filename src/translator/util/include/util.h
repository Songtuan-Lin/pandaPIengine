#ifndef _util_translator_inc_h_
#define _util_translator_inc_h_

#include <string>


struct Slot {
    int m, b, i;
    Slot(int m, int b, int i) {this->m = m; this->b = b; this->i = i;}
    std::string toString() {
        return "[" + std::to_string(m) + ";" + std::to_string(b) + ";" + std::to_string(i) + "]";
    }
};

#endif