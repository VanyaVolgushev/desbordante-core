#pragma once

#include <cstddef>

using FeatureIndex = unsigned int;
// O stands for Oredered (by frequency)
using OFeatureIndex = unsigned int;
// Consequence of a rule, O stands for oredered (by frequency)
struct OConsequence {
    OFeatureIndex feature;
    bool positive;
};
