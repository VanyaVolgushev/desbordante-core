#include "desar.h"

//INSERTED
/**
 * Numerical Association Rule Mining using DE, PSO...
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "DESolver.h"
#include "Setup.h"
#include "Archive.h"
#include "Problem.h"
#include "Squash.h"

//INSERTED

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
