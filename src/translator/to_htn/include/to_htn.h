#include "Model.h"
#include "task.h"
#include "method.h"
#include "htn.h"
#include "block.h"
#include "counter.h"
#include "insertion.h"
#include "match.h"
#include "meta.h"
#include "translator.h"
#include "validiation.h"
#include "traversal.h"

class HTNTranslator : public Translator {
    private:
        HTN h;
        vector<vector<vector<SlotMetaInfo>>> slotInfo;

    public:
        HTNTranslator(string htnFile, string planFile);
};