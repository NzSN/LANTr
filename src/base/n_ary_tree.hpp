#ifndef LANTR_BASE_N_ARY_TREE_H_
#define LANTR_BASE_N_ARY_TREE_H_

#include <memory>
#include <vector>

#include "base/utilities/bottom.hpp"


namespace LANTr::Base {

template<typename T>
class Tree {
public:
  using Children = std::vector<std::unique_ptr<T>>;

  // Iterator
  class iterator {
  public:
    iterator(T* root, Children::iterator iter):
      root_{root}, iter_(iter), isChildrenPart_{false} {

      Utility::Bottom::Unreachable(Utility::Bottom::NOT_IMPLEMENTED);
    }

    iterator& operator++() {
      if (!isChildrenPart_) {
        isChildrenPart_ = true;
      } else {
        ++iter_;
      }
      return *this;
    }

    iterator operator++(int) {
      return iterator(iter_++);
    }

    bool operator==(iterator& other) {
      return this->iter_ == other.iter_;
    }

    bool operator!=(iterator& other) {
      return this->iter_ != other.iter_;
    }

    T* operator*() {
      if (!isChildrenPart_) {
        return root_;
      } else {
        return iter_->get();
      }

    }
  private:
    bool isChildrenPart_;
    T* root_;
    Children::iterator iter_;
  };

  class const_iterator {
  public:
    explicit const_iterator(T* root, Children::const_iterator iter):
      root_{root}, iter_{iter}, isChildrenPart_{false} {}

    const_iterator& operator++() {
      if (!isChildrenPart_) {
        isChildrenPart_ = true;
      } else {
        ++iter_;
      }
      return *this;
    }

    bool operator==(const_iterator& other) {
      return this->iter_ == other.iter_;
    }

    bool operator!=(const_iterator& other) {
      return this->iter_ != other.iter_;
    }

    const T* operator*() const {
      if (!isChildrenPart_) {
        return root_;
      } else {
        return iter_->get();
      }
    }
  private:
    bool isChildrenPart_;
    T* root_;
    Children::const_iterator iter_;
  };

  auto begin() {
    return iterator(current_, children_.begin());
  }

  auto end() {
    return iterator(current_, children_.end());
  }

  auto begin() const {
    return const_iterator(current_, children_.cbegin());
  }

  auto end() const {
    return const_iterator(current_, children_.cend());
  }

  const T* Parent() const {
    return parent_;
  }

  void SetParent(T* parent) {
    parent_ = parent;
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

  Tree(T* current): current_(current) {}

protected:
  T* parent_;
  T* current_;
  Children children_;
};

} // LANTr::Base


#endif // LANTR_BASE_N_ARY_TREE_H_
