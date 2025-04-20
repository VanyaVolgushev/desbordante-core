#include "kingfisher.h"

#include <iostream>
#include <boost/math/distributions/hypergeometric.hpp>

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

    auto hasAnte = [](std::vector<unsigned> const& item_ids, model::NeARIDs const& r) {
        for(auto f: r.ante)
            if(std::find(item_ids.begin(), item_ids.end(), f) == item_ids.end())
                return false;
        return true;
    };
    auto hasCons = [](std::vector<unsigned> const& item_ids, model::NeARIDs const& r) {
        bool found = std::find(item_ids.begin(), item_ids.end(), r.cons) != item_ids.end();
        return r.cons_positive ? found : !found;
    };

    // 1) Count a, b, c, d
    std::size_t a=0, b=0, c=0, d=0;
    for(auto const& [itemset_index, itemset] : transactional_data_->GetTransactions()) {
        bool X = hasAnte(itemset.GetItemsIDs(), real_rule);
        bool Y = hasCons(itemset.GetItemsIDs(), real_rule);
        if     (X && Y)         ++a;
        else if(X && !Y)        ++b;
        else if(!X && Y)        ++c;
        else /* !X && !Y */     ++d;
    }

    // 2) Build Boost hypergeometric_distribution
    //    N = total transactions, r = successes in pop = a+c, n = draws = a+b
    const unsigned N = a + b + c + d;
    const unsigned r = a + c;
    const unsigned n = a + b;
    using boost::math::hypergeometric_distribution;
    hypergeometric_distribution<> dist(r, n, N);

    // 3) Compute observed cell‑probability
    double p_obs = boost::math::pdf(dist, a);

    // 4) Sum two‑sided P over all k with pdf(k) <= pdf(a)
    unsigned k_min = std::max<int>(0, int(n + r - N));
    unsigned k_max = std::min(n, r);
    double p_two_sided = 0.0;
    for(unsigned k = k_min; k <= k_max; ++k) {
        double p_k = boost::math::pdf(dist, k);
        if(p_k <= p_obs)
            p_two_sided += p_k;
    }

    return p_two_sided;
}

unsigned long long Kingfisher::ExecuteInternal() {
    // CALCS MINIMUM FREQUENCY AND CREATES TREE WITH ATTRS THAT SATISFY IT
    feature_frequency_order_ = {1, 2, 3, 0};  // hard coded for now
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
