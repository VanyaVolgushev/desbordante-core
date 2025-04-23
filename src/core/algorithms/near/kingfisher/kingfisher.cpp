#include "kingfisher.h"

#include <iostream>

#include <boost/math/distributions/hypergeometric.hpp>

#include "model/table/position_list_index.h"
#include "algorithms/near/near_discovery.h"
#include "config/names_and_descriptions.h"
#include "option_using.h"

namespace algos {

Kingfisher::Kingfisher() : NeARDiscovery({}) {
    RegisterOptions();
}

void Kingfisher::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    RegisterOption(Option{&max_rules_, kMaxRules, kDMaxRules, 10u});
    RegisterOption(Option{&max_p_, kMaxPValue, kDMaxPValue, 1.2e-8});
}

void Kingfisher::ResetState() {
    // TODO: implement this
}

void Kingfisher::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kMaxPValue, kMaxRules});
}

template <typename T>
std::string Kingfisher::VectorToString(std::vector<T> const& vec) {
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

// NodeAdress is provided with indices in order of frequency
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
    model::NeARIDs real_rule = rule.UndoOrder(feature_frequency_order_);

    auto hasAnte = [&](std::vector<unsigned> const& ids, model::NeARIDs const& r) {
        for (auto f : r.ante)
            if (std::find(ids.begin(), ids.end(), f) == ids.end()) return false;
        return true;
    };
    auto hasCons = [&](std::vector<unsigned> const& ids, model::NeARIDs const& r) {
        bool found = std::find(ids.begin(), ids.end(), r.cons) != ids.end();
        return r.cons_positive ? found : !found;
    };

    // Count a, b, c, d
    std::size_t a = 0, b = 0, c = 0, d = 0;
    for (auto const& [_, tx] : transactional_data_->GetTransactions()) {
        bool X = hasAnte(tx.GetItemsIDs(), real_rule);
        bool Y = hasCons(tx.GetItemsIDs(), real_rule);
        if (X && Y)
            ++a;
        else if (X && !Y)
            ++b;
        else if (!X && Y)
            ++c;
        else
            ++d;
    }

    unsigned const N = a + b + c + d;
    unsigned const r = a + c;
    unsigned const n = a + b;

    using boost::math::hypergeometric_distribution;
    hypergeometric_distribution<> dist(r, n, N);

    // Find valid lower bound for the quantile
    int const support_min = std::max(0, int(n + r - N));
    int const quantile = int(a) - 1;

    if (quantile < support_min) {
        // Whole right tail = 1
        return 1.0;
    }

    return boost::math::cdf(boost::math::complement(dist, static_cast<double>(quantile)));
}

// TODO: PLI
std::vector<FeatureIndex> Kingfisher::GetFeatureFrequency() const {
    std::vector<std::pair<size_t, FeatureIndex>> feature_frequency;
    feature_frequency.reserve(transactional_data_->GetUniverseSize());
    // iota
    for (size_t i = 0; i < transactional_data_->GetUniverseSize(); ++i) {
        feature_frequency.emplace_back(0, i);
    }
    for (auto const& [_, itemset] : transactional_data_->GetTransactions()) {
        for (auto const& item : itemset.GetItemsIDs()) {
            ++feature_frequency[item].first;
        }
    }

    std::sort(feature_frequency.begin(), feature_frequency.end(),
              [](auto const& a, auto const& b) { return a.first < b.first; });

    std::vector<FeatureIndex> feature_frequency_order;
    feature_frequency_order.reserve(feature_frequency.size());
    for (auto const& [_, index] : feature_frequency) {
        feature_frequency_order.push_back(index);
    }
    return feature_frequency_order;
}

unsigned long long Kingfisher::ExecuteInternal() {
    // TODO: CALCS MINIMUM FREQUENCY AND CREATES TREE WITH ATTRS THAT SATISFY IT
    std::cout << VectorToString(transactional_data_->GetItemUniverse()) << std::endl; // DEBUG
    feature_frequency_order_ = GetFeatureFrequency();

    using namespace std::placeholders;
    auto tree = kingfisher::CandidatePrefixTree{
            transactional_data_->GetUniverseSize(),
            std::bind(&Kingfisher::GetLowerBound1, this, _1),
            std::bind(&Kingfisher::GetLowerBound2, this, _1, _2, _3),
            std::bind(&Kingfisher::GetLowerBound3, this, _1, _2, _3),
            std::bind(&Kingfisher::GetFishersP, this, _1),
            max_p_,
            max_rules_};

    near_collection_ = tree.GetNeARIDs(feature_frequency_order_);

    return 0;
}

}  // namespace algos
