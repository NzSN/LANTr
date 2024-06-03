#ifndef LANTR_BASE_TREE_LAYER_H_
#define LANTR_BASE_TREE_LAYER_H_

#include "base/n_ary_tree.hpp"
#include "base/tree_concepts.hpp"
#include "base/utilities/assert.hpp"
#include "base/utilities/bottom.hpp"
#include <queue>

namespace LANTr::Base {

template<typename U, typename L>
requires std::derived_from<L, Tree<L>> ||
         TreeConcepts::AntlrTree<L>
class TreeLayer: public Tree<U> {
public:
  TreeLayer(U* upper, L* lower): Tree<U>(upper), lower_(lower) {
    static_assert(std::derived_from<U, TreeLayer<U,L>>);
  }

  static std::unique_ptr<U> BuildFrom(L* lower) {
    ASSERT(lower != nullptr, "BuildFrom nullptr");

    auto root = std::make_unique<U>(lower);
    for (auto& c: TreeConcepts::GetChildren(lower)) {
      root->AddChild(BuildFrom(TreeConcepts::GetRawPtr(c)));
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
      if (child->lower_ != TreeConcepts::GetRawPtr(lowerChild) ||
          !child->IsLayerEquivalent()) {
        return false;
      }
    }

    return true;
  }

  void Synchronize() {
    Utility::Bottom::Unreachable(Utility::Bottom::NOT_IMPLEMENTED);

    // Update InvalidateState for all SubNodes
    // UpdateInvalidateState(this);

    // Rebuild tree from those Invalidated nodes
    // RebuildAllInvalidateNodes(this);
  }

private:
  enum InvalidateState {
    VALID,
    PARTIAL_VALID,
    INVALID,
  };

  InvalidateState state_ = VALID;
  std::queue<TreeLayer*> work_list_;

  void RebuildAllInvalidateNodes(TreeLayer* node) {
    ASSERT(work_list_.empty(), "Invalidated state of tree is not clean");
  }

  InvalidateState Stepping(TreeLayer* node) {
    if (node->size() !=
        TreeConcepts::GetChildren(node->lower_).size()) {
      return PARTIAL_VALID;
    }
    auto zipChild = std::views::zip(
      node->children_,
      TreeConcepts::GetChildren(node->lower_));
    for (auto [child, lowerChild]: zipChild) {
      if (child->lower_ != TreeConcepts::GetRawPtr(lowerChild)) {
        child->state_ = INVALID;
        return PARTIAL_VALID;
      }
    }

    return VALID;
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

  void UpdateInvalidateState(TreeLayer* node) {
    ASSERT(work_list_.empty(),
           "Work list should be empty at initial of invalidate");

    // Use the node pointed by argument as root node
    // to process invalidate.
    work_list_.push(node);
    return DoUpdateInvalidateState();
  }

protected:
  friend struct TreeLayerTester;
  L* lower_;
};


} // LANTr::Base

#endif /* LANTR_BASE_TREE_LAYER_H_ */
