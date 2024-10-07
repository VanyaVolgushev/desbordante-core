
#pragma once

#include <enum.h>

namespace algos::des {

    BETTER_ENUM(DifferentialStrategy, char, best1Exp = 0, rand1Exp, randToBest1Exp, best2Exp, rand2Exp,
                                                          best1Bin, rand1Bin, randToBest1Bin, best2Bin, rand2Bin); //TODO: add descriptions of each strategy
}  // namespace algos
