#include "Model.h"
#include "task.h"
#include "method.h"
#include "translator.h"

class HTNTranslator : public Translator {
    private:
        vector<PrimitiveTask> posTasks;

    public:
        HTNTranslator(string htnFile, string planFile);
};