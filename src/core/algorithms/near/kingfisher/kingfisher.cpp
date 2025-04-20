#include "kingfisher.h"

#include <iostream>

#include "algorithms/near/near_discovery.h"
#include "config/names_and_descriptions.h"
#include "option_using.h"

namespace algos {

Kingfisher::Kingfisher() : NeARDiscovery({}) {
    RegisterOptions();
}

void Kingfisher::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    RegisterOption(Option{&max_p_, kMaxPValue, kDMaxPValue, 1.2e-8});
}

void Kingfisher::ResetState() {
    // TODO: implement this
}

void Kingfisher::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kMaxPValue});
}

std::string Kingfisher::VectorToString(std::vector<FeatureIndex> const& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];  // Convert FeatureIndex to string
        if (i < vec.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

double Kingfisher::GetLowerBound1(OFeatureIndex feature) {
    std::cout << "Lower bound 1 for feat " + std::to_string(feature_frequency_order_[feature]) +
                         " is: ";
    // double bound;
    // std::cin >> bound;
    return 0;
}

double Kingfisher::GetLowerBound2(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                                  bool cons_positive) {
    // TODO: проверка на то что выражения под факториалами неотрицательны
    std::cout << "Lower bound 2 for " +
                         VectorToString(node_addr.ToFeatures(feature_frequency_order_)) + " at " +
                         std::to_string(feature_frequency_order_[cons_index]) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
    //
    // double bound;
    // std::cin >> bound;
    return 0;
}

double Kingfisher::GetLowerBound3(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                                  bool cons_positive) {
    std::cout << "Lower bound 3 for " +
                         VectorToString(node_addr.ToFeatures(feature_frequency_order_)) + " at " +
                         std::to_string(feature_frequency_order_[cons_index]) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
    //
    // double bound;
    // std::cin >> bound;
    return 0;
}

double Kingfisher::GetFishersP(model::NeARIDs const& rule) {
    for (const auto& [transaction_id, itemset] : transactional_data_->GetTransactions()) {
        feature_frequency_order_[]
        itemset.GetItemsIDs()
    }

    return 0;
}

unsigned long long Kingfisher::ExecuteInternal() {
    // CALCS MINIMUM FREQUENCY AND CREATES TREE WITH ATTRS THAT SATISFY IT
    feature_frequency_order_ = {1, 2, 3, 0}; // hard coded for now
    assert(feature_frequency_order_.size() == transactional_data_->GetUniverseSize());
    
    using namespace std::placeholders;
    auto tree = kingfisher::CandidatePrefixTree{
            transactional_data_->GetUniverseSize(),
            std::bind(&Kingfisher::GetLowerBound1, this, _1),
            std::bind(&Kingfisher::GetLowerBound2, this, _1, _2, _3),
            std::bind(&Kingfisher::GetLowerBound3, this, _1, _2, _3),
            std::bind(&Kingfisher::GetFishersP, this, _1),
            max_p_};

    return 0;
}

}  // namespace algos
