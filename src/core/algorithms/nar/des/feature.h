#pragma once

#include "model/types/type.h"

namespace algos::des {

class Feature {
protected:
    std::unique_ptr<const model::Type> type_; // I don't understand why it's constant, but it's the same in TypedColumnData.
};

class CategoricalFeature : public Feature {

};

class RealFeature : public Feature {

};

class IntegerFeature : public Feature {

};

} // namespace algos::cfd
