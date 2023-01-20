#include "to_htn.h"

HTNTranslator::HTNTranslator(string htnFile, string planFile) : Translator(htnFile, planFile) {
    MatchingProps matchingProps(this->plan.size(), this->h.getNumProps());
}