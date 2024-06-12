#ifndef LANTR_BASE_N_ARY_TREE_H_
#define LANTR_BASE_N_ARY_TREE_H_

#include <concepts>
#include <memory>
#include <vector>

#include "base/tree_concepts.hpp"
#include "base/utilities/assert.hpp"
#include "base/utilities/bottom.hpp"


namespace LANTr::Base {

template<typename T>
class Tree {
public:
  using Children = std::vector<std::unique_ptr<T>>;

  // Iterate over a Tree in Pre-Order.
  class iterator {
  public:

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;

    iterator(T* node, bool isEnd = false):
     isEnd_{isEnd}, node_{node}  {

      node_ = isEnd_ ? nullptr : node_;
      ASSERT((!isEnd_ && node_ != nullptr) ||
             (isEnd && node_ == nullptr),
             "Iterate over a memory area pointed by nullptr");
    }

    // Pre-Order Traversal.
    iterator& operator++() {
      if (isEnd_) {
        return *this;
      }

      Tree<T>* treeNode = static_cast<Tree<T>*>(node_);

      if (!treeNode->children_.empty()) {
        // The node will be the next to be accessed.
        node_ = treeNode->children_.front().get();

        // Push right side of childs into stack in reverse order.
        auto begin = treeNode->children_.rbegin();
        auto end   = treeNode->children_.rend() - 1;
        for (typename Children::reverse_iterator iter = begin;
            iter != end; ++iter) {
          accesses_.push_back(iter->get());
        }
      } else if (!accesses_.empty()) {
        node_ = accesses_.back();
        accesses_.pop_back();
      } else {
        isEnd_ = true;
        node_ = nullptr;
      }

      return *this;
    }

    iterator operator++(int) {
      auto iter = *this;
      this->operator++();
      return iter;
    }

    bool operator==(const iterator& other) const {
      return this->node_ == other.node_ &&
        this->isEnd_ == other.isEnd_;
    }

    bool operator!=(const iterator& other) const {
      return !this->operator==(other);
    }

    T& operator*() const {
      return *node_;
    }

  private:
    bool isEnd_;
    T* node_;
    std::vector<T*> accesses_;
  };

  auto begin() {
    return iterator(current_);
  }

  auto end() {
    return iterator(current_, true);
  }

  T* Parent() const {
    return parent_;
  }

  void SetParent(T* parent) {
    parent_ = parent;
  }

  bool IsDescdentOf(Tree<T>& other) {
    auto parent = parent_;

    while (parent) {
      if (parent->current_ == other.current_) {
        return true;
      }
      parent = parent->parent_;
    }

    return false;
  }

  void AddChild(std::unique_ptr<T> child) {
    child->SetParent(current_);
    children_.push_back(std::move(child));
  }

  [[nodiscard]] bool HasChild() const {
    return children_.size() > 0;
  }

  [[nodiscard]] const Children& GetChildren() const {
    return children_;
  }

  [[nodiscard]] Children& GetChildren() {
    return children_;
  }

  size_t size() const {
    return children_.size();
  }

  Tree(T* current):
    parent_{nullptr}, current_{current} {

    static_assert(TreeConcepts::NAryTree<T>);
    static_assert(std::derived_from<T, Tree<T>>);
  }

protected:
  T* parent_;
  T* current_;

  Children children_;
};

template<typename T>
int32_t GetNumOfNodes(Tree<T>& tree) {
  int32_t count = 0;
  for ([[maybe_unused]] auto& node: tree) {
    ++count;
  }

  return count;
}

} // LANTr::Base


#endif // LANTR_BASE_N_ARY_TREE_H_
