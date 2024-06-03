#ifndef LANTR_BASE_TREE_LAYER_H_
#define LANTR_BASE_TREE_LAYER_H_

#include "base/n_ary_tree.hpp"
#include "base/tree_concepts.hpp"
#include "base/utilities/assert.hpp"

namespace LANTr::Base {

template<typename U, typename L>
requires std::derived_from<L, Tree<L>> ||
         TreeConcepts::AntlrTree<L>
class TreeLayer: public Tree<U> {
public:
  TreeLayer(U* upper, L* lower): Tree<U>(upper), lower_(lower) {}

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

  }

protected:
  struct TreeLayerTester;
  L* lower_;
};


} // LANTr::Base

#endif /* LANTR_BASE_TREE_LAYER_H_ */
