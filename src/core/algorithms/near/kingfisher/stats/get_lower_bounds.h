#pragma once

#include <iostream>

#include "algorithms/near/kingfisher/node_adress.h"
#include "algorithms/near/kingfisher/util/vector_to_string.h"
#include "algorithms/near/types.h"
#include "get_frequency.h"

#include <string.h>

namespace kingfisher {

double GetLowerBound1(FeatureIndex feature) {
    std::cout << "Lower bound 1 for feat " + std::to_string(feature) + " is: ";
    // double bound;
    // std::cin >> bound;
    return 0;
}

double GetLowerBound2(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                      bool cons_positive) {
    // TODO: проверка на то что выражения под факториалами неотрицательны
    (void)node_addr;  // Mark node_addr as unused to avoid warnings
    std::cout << "Lower bound 2 for at " + std::to_string(cons_index) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
    // double bound;
    // std::cin >> bound;
    return 0;
}

// NodeAdress is provided with indices in order of frequency
double GetLowerBound3(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                      bool cons_positive) {
    (void)node_addr;  // Mark node_addr as unused to avoid warnings
    std::cout << "Lower bound 3 for at " + std::to_string(cons_index) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
    // double bound;
    // std::cin >> bound;
    return 0;
}

}  // namespace kingfisher
