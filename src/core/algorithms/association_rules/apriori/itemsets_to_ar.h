#pragma once

#include "algorithms/association_rules/ar_algorithm.h"
#include "rule_node.h"

namespace algos {

class ItemsetsToAR : public ARAlgorithm {
private:
    RuleNode rule_root_;

    virtual double GetSupport(std::vector<unsigned> const& frequent_itemset) const = 0;
    bool GenerateRuleLevel(std::vector<unsigned> const& frequent_itemset, double support,
                           unsigned level_number);
    bool MergeRules(std::vector<unsigned> const& frequent_itemset, double support, RuleNode* node);
    static void UpdatePath(std::stack<RuleNode*>& path, std::list<RuleNode>& vertices);
    void ResetState() override;
    virtual void ResetStateAr() = 0;

protected:
    void GenerateRulesFrom(std::vector<unsigned> const& frequent_itemset, double support);
    virtual unsigned long long FindFrequent() = 0;
    unsigned long long ExecuteInternal() final;

public:
    explicit ItemsetsToAR(std::vector<std::string_view> phase_names);
};

} //namespace algos