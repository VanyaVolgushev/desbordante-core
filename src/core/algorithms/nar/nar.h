#pragma once

#include <vector>

#include "attribute.h"


namespace algos::nar {

class NAR {
public:

    NAR();
    double fitness;
    double support;
    double confidence;

    std::vector<Attribute> ante;		///< antecedent part of the association rule
    std::vector<Attribute> cons;		///< consequent part of the association rule
};

} // model
