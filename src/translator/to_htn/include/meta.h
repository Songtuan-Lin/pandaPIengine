#ifndef _meta_inc_h_
#define _meta_inc_h_

#include "assert.h"
#include "insertion.h"
#include "block.h"
#include "counter.h"

struct SlotTranslation {
    PropsForInsertion propsForInsertion;
    PrimsForInsertion primsForInsertion;
    CompForInsertion compForInsertion;
    MethodsForInsertion methodsForInsertion;
};

#endif