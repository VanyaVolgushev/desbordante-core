#pragma once

#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "algorithms/association_rules/apriori/candidate_hash_tree.h"
#include "algorithms/association_rules/apriori/itemset_node.h"
#include "algorithms/association_rules/apriori/itemsets_to_ar.h"
#include "model/transaction/itemset.h"

namespace algos {

class Apriori : public ItemsetsToAR {
private:
    // TODO(alexandrsmirn): попробовать убрать из полей, и создавать просто в методе
    // GenerateAllRules
    std::unique_ptr<CandidateHashTree> candidate_hash_tree_;

    ItemsetNode itemset_root_;

    std::unordered_map<ItemsetNode*, std::list<ItemsetNode>> candidates_;
    unsigned level_num_ = 1;

    bool GenerateNextCandidateLevel();

    bool CanBePruned(std::vector<unsigned> const& itemset);
    void GenerateCandidates(std::vector<ItemsetNode>& children);
    void CreateFirstLevelCandidates();
    void AppendToTree();

    static void UpdatePath(std::stack<ItemsetNode*>& path, std::vector<ItemsetNode>& vertices);
    static void UpdatePath(std::queue<ItemsetNode const*>& path, std::vector<ItemsetNode> const& vertices);
    static void UpdatePath(std::stack<ItemsetNode const*>& path, std::vector<ItemsetNode> const& vertices);

    double GetSupport(std::vector<unsigned> const& frequent_itemset) const override;
    unsigned long long GenerateAllRules() override;
    unsigned long long FindFrequent() override;

    void ResetStateAr() final;

public:
    Apriori();
    std::list<std::set<std::string>> GetFrequentList() const;  // for debugging and testing
};

}  // namespace algos
