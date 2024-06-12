#ifndef LANTR_BASE_TREE_LAYER_H_
#define LANTR_BASE_TREE_LAYER_H_

#include <queue>
#include <algorithm>

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
    // RebuildAllInvalidateNodes();
  }

private:
  enum InvalidateState {
    FIRST_STATE,
    VALID = FIRST_STATE,
    PARTIAL_VALID,
    INVALID,
    END_STATE
  };

  InvalidateState state_ = VALID;
  std::queue<TreeLayer*> work_list_;

  void CorrectPartialNode(TreeLayer& node) {

  }

  void CorrectInvalidNode(TreeLayer& node) {

  }

  void CorrectRelations() {
    ASSERT(state_ == VALID,
           "Assumption of Valid of root "
           "node of subtree is violated");
    ASSERT(work_list_.empty(), "Invalidated state of tree is not clean");

    std::for_each(this->begin(), this->end(),
                  [&](TreeLayer& tree) {
                    switch (tree.state_) {
                    case VALID:
                      break;
                    case INVALID:
                      CorrectInvalidNode(tree);
                      break;
                    case PARTIAL_VALID:
                      CorrectPartialNode(tree);
                      break;
                    default:
                      ASSERT(tree.state_ >= FIRST_STATE &&
                             tree.state_ < END_STATE,
                             "Invalid state");
                    }
                  });
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
