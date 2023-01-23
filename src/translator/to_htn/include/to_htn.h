#include "Model.h"
#include "task.h"
#include "method.h"
#include "htn.h"
#include "block.h"
#include "compound.h"
#include "counter.h"
#include "insertion.h"
#include "match.h"
#include "meta.h"
#include "translator.h"

class HTNTranslator : public Translator {
    private:
        HTN h;
        vector<vector<vector<SlotTranslation>>> slotTranslations;
        vector<Counter> countersForMethods;

    public:
        HTNTranslator(string htnFile, string planFile);
};