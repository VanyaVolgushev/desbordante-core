#include <easylogging++.h>

#include "itemsets_to_ar.h"
#include "itemset_node.h"

namespace algos {

ItemsetsToAR::ItemsetsToAR(std::vector<std::string_view> phase_names)
        : ARAlgorithm(std::move(phase_names)) {}


void ItemsetsToAR::UpdatePath(std::stack<RuleNode*>& path, std::list<RuleNode>& vertices) {
    for (auto iter = vertices.rbegin(); iter != vertices.rend(); ++iter) {
        RuleNode* node_ptr = &(*iter);
        path.push(node_ptr);
    }
}

void ItemsetsToAR::ResetState() {
    ar_collection_.clear();
    ResetStateAr();
}

void ItemsetsToAR::GenerateRulesFrom(std::vector<unsigned> const& frequent_itemset, double support) {
    rule_root_.children.clear();
    for (auto item_id : frequent_itemset) {
        std::vector<unsigned> rhs{item_id};
        std::vector<unsigned> lhs;
        std::set_difference(frequent_itemset.begin(), frequent_itemset.end(), rhs.begin(),
                            rhs.end(), std::back_inserter(lhs));
        auto const lhs_support = GetSupport(lhs);
        auto const confidence = support / lhs_support;
        if (confidence >= minconf_) {
            auto const& new_ar =
                    ar_collection_.emplace_back(std::move(lhs), std::move(rhs), confidence);
            rule_root_.children.emplace_back(new_ar);
        }
    }
    if (rule_root_.children.empty()) {
        return;
    }

    unsigned level_number = 2;
    while (GenerateRuleLevel(frequent_itemset, support, level_number)) {
        ++level_number;
    }
}

bool ItemsetsToAR::GenerateRuleLevel(std::vector<unsigned> const& frequent_itemset, double support,unsigned level_number) {
    bool generated_any = false;
    std::stack<RuleNode*> path;
    path.push(&rule_root_);

    assert(level_number >= 2);
    while (!path.empty()) {
        // TODO(alexandrsmirn) попробовать как-то синхронизировать путь с генерацией
        auto node = path.top();
        path.pop();
        if (node->rule.right.size() == level_number - 2) {  // levelNumber is at least 2
            generated_any = MergeRules(frequent_itemset, support, node);
        } else {
            UpdatePath(path, node->children);
        }
    }

    return generated_any;
}

bool ItemsetsToAR::MergeRules(std::vector<unsigned> const& frequent_itemset, double support,RuleNode* node) {
    auto& children = node->children;
    bool is_rule_produced = false;

    auto const last_child_iter = std::prev(children.end());
    for (auto child_iter = children.begin(); child_iter != last_child_iter; ++child_iter) {
        for (auto child_right_sibling_iter = std::next(child_iter);
        child_right_sibling_iter != children.end(); ++child_right_sibling_iter) {
            std::vector<unsigned> rhs = child_iter->rule.right;
            rhs.push_back(child_right_sibling_iter->rule.right.back());
            if (rhs.size() == frequent_itemset.size()) {
                // in this case the LHS is empty
                continue;
            }

            std::vector<unsigned> lhs;
            std::set_difference(frequent_itemset.begin(), frequent_itemset.end(), rhs.begin(),
                                rhs.end(), std::back_inserter(lhs));

            auto const lhs_support = GetSupport(lhs);
            auto const confidence = support / lhs_support;
            if (confidence >= minconf_) {
                auto const& new_ar =
                        ar_collection_.emplace_back(std::move(lhs), std::move(rhs), confidence);
                child_iter->children.emplace_back(new_ar);
                is_rule_produced = true;
            }
        }
    }
    return is_rule_produced;
}

unsigned long long ItemsetsToAR::ExecuteInternal() {
    auto time = FindFrequent();
    time += GenerateAllRules();

    LOG(INFO) << "> Count of association rules: " << ar_collection_.size();
    return time;
}


} //namespace algos