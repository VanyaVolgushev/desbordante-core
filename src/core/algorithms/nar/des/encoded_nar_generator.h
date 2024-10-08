#pragma once

#include <random>

#include "encoded_nar.h"
#include "algorithms/nar/feature_bounds.h"

namespace algos::des {

class EncodedNARGenerator
{
private:
    using FeatureDomain = std::shared_ptr<const FeatureBounds>;
    std::vector<FeatureDomain> feature_domains_;
    std::random_device rdevice_;

public:
    EncodedNAR GenerateUniformRandom() const;

    explicit EncodedNARGenerator(std::vector<FeatureDomain> feature_domains);
};

} // namespace algos::des
