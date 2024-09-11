#include "desar.h"

namespace algos {

DESAR::DESAR() : ARAlgorithm({}) {}

unsigned long long DESAR::GenerateAllRules() {
    return 1;
}
void DESAR::ResetState()
{
    //everything = 0 and delete
}

unsigned long long DESAR::ExecuteInternal() {
    GenerateAllRules();
    return 1337;
}

void DESAR::RegisterOptions()
{

}
}  // namespace algos
