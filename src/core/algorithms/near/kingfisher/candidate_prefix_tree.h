#pragma once

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "algorithms/near/near.h"
#include "algorithms/near/near_types.h"
#include "node.h"

namespace algos {

class NodeAdress {
private:
    std::vector<OrderedFeatureIndex> vec_;

public:
    NodeAdress(std::vector<OrderedFeatureIndex> vec);

    NodeAdress(OrderedFeatureIndex ordered_feat_index) {
        vec_ = {ordered_feat_index};
    }

    std::vector<OrderedFeatureIndex> Get() const;
    std::vector<OrderedFeatureIndex> GetExcept(size_t at) const;
    OrderedFeatureIndex PopBack();
    OrderedFeatureIndex Front();
    size_t Size() const;
    bool Empty() const;
    std::vector<FeatureIndex> ToFeatures(std::vector<FeatureIndex> const& order) const;
    std::string ToString() const;
};

using GetFishersP = std::function<double(model::NeARIDs const& rule)>;

// Returns the best possible Fisher's p-value for a node and its children given that cons_index is
// the consequence
using GetLowerBound1 = std::function<double(OrderedFeatureIndex index)>;
using GetLowerBound2or3 = std::function<double(NodeAdress const& node_addr,
                                               OrderedFeatureIndex cons_index, bool cons_positive)>;

// All features inside this class are indexed in order of their frequency
class CandidatePrefixTree {
private:
    RoutingNode root_{true};
    size_t feat_count_;
    size_t depth_;
    std::queue<NodeAdress> bfs_queue_;
    std::vector<model::NeARIDs> k_best_;
    double max_goodness_;

    GetLowerBound1 lower_bound1_;
    GetLowerBound2or3 lower_bound2_;
    GetLowerBound2or3 lower_bound3_;
    GetFishersP get_p_;
    double max_p_;

    Node& GetNode(NodeAdress adress);
    void IncreaseDepth();
    void CheckDepth1();
    void PerformBFS();

public:
    CandidatePrefixTree(size_t feat_count, GetLowerBound1 lower_bound1,
                        GetLowerBound2or3 lower_bound2, GetLowerBound2or3 lower_bound3,
                        GetFishersP goodness_measure, double max_p);
    std::vector<model::NeARIDs>& GetNeARIDs();
};

}  // namespace algos
