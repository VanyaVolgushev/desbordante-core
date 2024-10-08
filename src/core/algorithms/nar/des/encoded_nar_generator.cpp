#include "iostream"
#include "encoded_nar_generator.h"

namespace algos::des {

EncodedNARGenerator::EncodedNARGenerator(std::vector<FeatureDomain> feature_domains) {
    feature_domains_ = feature_domains;
}

EncodedNAR EncodedNARGenerator::GenerateUniformRandom() const {
    //TODO
}

} // namespace algos::des
