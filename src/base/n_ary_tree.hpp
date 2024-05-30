#ifndef LANTR_BASE_N_ARY_TREE_H_
#define LANTR_BASE_N_ARY_TREE_H_

#include <memory>
#include <vector>

namespace LANTr::Base {

template<typename T>
class Tree {
public:
  using Children = std::vector<std::unique_ptr<T>>;

  // Iterator
  class iterator {
  public:
    iterator(Children::iterator iter):
      iter_(iter) {}

    iterator& operator++() {
      ++iter_;
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
      return iter_->get();
    }
  private:
    Children::iterator iter_;
  };

  class const_iterator {
  public:
    explicit const_iterator(Children::const_iterator iter):
      iter_{iter} {}

    const_iterator& operator++() {
      ++iter_;
      return *this;
    }

    bool operator==(const_iterator& other) {
      return this->iter_ == other.iter_;
    }

    bool operator!=(const_iterator& other) {
      return this->iter_ != other.iter_;
    }

    const T* operator*() const {
      return iter_->get();
    }
  private:
    Children::const_iterator iter_;
  };

  auto begin() {
    return iterator(children_.begin());
  }

  auto end() {
    return iterator(children_.end());
  }

  auto begin() const {
    return const_iterator(children_.cbegin());
  }

  auto end() const {
    return const_iterator(children_.cend());
  }

  const T* Parent() const {
    return parent_;
  }

  void SetParent(T* parent) {
    parent_ = parent;
  }

  void AddChild(std::unique_ptr<T> child) {
    children_.push_back(std::move(child));
  }

  size_t size() const {
    return children_.size();
  }

protected:
  T* parent_;
  Children children_;
};


} // LANTr::Base


#endif // LANTR_BASE_N_ARY_TREE_H_
