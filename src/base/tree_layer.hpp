#ifndef LANTR_BASE_TREE_LAYER_H_
#define LANTR_BASE_TREE_LAYER_H_

#include <format>
#include <functional>
#include <iostream>
#include <queue>
#include <algorithm>

#include "base/antlr4_tree_concepts.hpp"
#include "base/n_ary_tree.hpp"
#include "base/concepts.hpp"
#include "base/tree_concepts.hpp"
#include "base/utilities/assert.hpp"
#include "base/utilities/bottom.hpp"

namespace LANTr::Base {

template<typename U, typename L>
class TreeLayer: public Tree<U> {
public:
  TreeLayer(U* upper, L* lower): Tree<U>(upper), lower_(lower) {
    static_assert(std::derived_from<U, TreeLayer<U,L>>);
    static_assert(TreeConcepts::NAryTree<U>);
    static_assert(TreeConcepts::NAryTree<L>);
  }

  static std::unique_ptr<U> BuildFrom(L* lower) {
    std::cout << std::format("Hello {} !\n", "World");
    ASSERT(lower != nullptr, "BuildFrom nullptr");

    auto root = std::make_unique<U>(lower);
    for (auto& c: TreeConcepts::GetChildren(lower)) {
      root->AddChild(BuildFrom(Concepts::GetRawPtr(c)));
    }

    return root;
  }

  bool IsLayerEquivalent() const {
    if (this->size() != TreeConcepts::GetChildren(lower_).size()) {
      return false;
    }

    auto zipChild = std::views::zip(
      this->children_,
      TreeConcepts::GetChildren(lower_));
    for (auto [child, lowerChild]: zipChild) {
      if (child->lower_ != Concepts::GetRawPtr(lowerChild) ||
          !child->IsLayerEquivalent()) {
        return false;
      }
    }

    return true;
  }

  void Synchronize() {
    // Update InvalidateState for all SubNodes
    UpdateInvalidateState();

    // Rebuild tree from those Invalidated nodes
    // CorrectRelations();
  }

private:
  enum InvalidateState {
    FIRST_STATE,
    VALID = FIRST_STATE,
    // There are some direct or indirect
    // nodes not in VALID state.
    PARTIAL_VALID,
    // Mapping from upper to lower of a INVALID node
    // and all it's direct and indirect childrent are
    // INVALID.
    INVALID,
    END_STATE
  };

  bool SwitchToValid() {
    if (this->state_ == VALID) {
      return true;
    }

    bool ableToSwitch =
      std::find_if(this->children_.begin(), this->children_.end(),
                 [](TreeLayer& u) { u.state_ != VALID; })
      != this->children_.end();

    if (!ableToSwitch) {
      return false;
    }

    this->state_ = VALID;

    // Maintain invariant of state
    if (this->parent_) {
      reinterpret_cast<TreeLayer*>(this->parent_)->SwitchToValid();
    }

    return true;
  }

  bool SwitchToPartialValid() {
    if (this->state_ == PARTIAL_VALID) {
      return true;
    }

    this->state_ = PARTIAL_VALID;


  }

  void SwitchToInvalid() {

  }

  InvalidateState state_ = VALID;
  std::queue<TreeLayer*> work_list_;

  void CorrectPartialNode(TreeLayer& node) {
    ASSERT(TreeConcepts::NumOfChildren(&node) !=
           TreeConcepts::NumOfChildren(node.lower_));
    ASSERT(node.state_ == PARTIAL_VALID);




  }

  void CorrectInvalidNode(TreeLayer& node) {

  }

  void CorrectStep(TreeLayer& node) {
    // Each Correcting Step must start from a
    // VALID or PARTIAL_VALID node.
    ASSERT(node.state_ == VALID ||
           node.state_ == PARTIAL_VALID);
  }

  void CorrectRelations() {
    ASSERT(state_ == VALID);
    ASSERT(work_list_.empty());

    std::for_each(this->begin(), this->end(),
                  std::bind(&TreeLayer::CorrectStep,
                            this, std::placeholders::_1));

    ASSERT_SLOW(std::find_if(
                  this->begin(), this->end(),
                  [](TreeLayer& layer) -> bool{
                    return layer.state_ != VALID;
                  }) == this->end(),
                "Some nodes still in invalid state after correct");
  }

  InvalidateState Stepping(TreeLayer* node) {
    InvalidateState state = VALID;

    if (node->size() !=
        TreeConcepts::GetChildren(node->lower_).size()) {
      state = PARTIAL_VALID;
    } else {
      auto zipChild = std::views::zip(
        node->children_,
        TreeConcepts::GetChildren(node->lower_));
      for (auto [child, lowerChild]: zipChild) {
        if (child->lower_ != Concepts::GetRawPtr(lowerChild)) {
          child->state_ = INVALID;
          state = PARTIAL_VALID;
        }
      }
    }

    return state;
  }

  void UpdateInvalidateStateStep(TreeLayer* current) {
    current->state_ = Stepping(current);

    for (auto& child: current->GetChildren()) {
      if (child->state_ == INVALID) {
        continue;
      }
      work_list_.push(child.get());
    }
  }

  void DoUpdateInvalidateState() {
    while (!work_list_.empty()) {
      TreeLayer* current = work_list_.front();
      UpdateInvalidateStateStep(current);
      work_list_.pop();
    }

    ASSERT(work_list_.empty(),
           "Work list should be empty at end of invalidate algorithm");
  }

  void UpdateInvalidateState() {
    ASSERT(work_list_.empty(),
           "Work list should be empty at initial of invalidate");

    // Use the node pointed by argument as root node
    // to process invalidate.
    work_list_.push(this);
    return DoUpdateInvalidateState();
  }

protected:
  friend struct TreeLayerTester;
  L* lower_;
};


} // LANTr::Base

#endif /* LANTR_BASE_TREE_LAYER_H_ */
