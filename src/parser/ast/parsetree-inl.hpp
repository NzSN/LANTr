#ifndef LANTR_PARSER_AST_PARSETREE_IMPL
#define LANTR_PARSER_AST_PARSETREE_IMPL

#include "antlr4-runtime.h"
#include "parsetree.hpp"

namespace LANTr::Parser::AST {

template<typename T>
class ParseTree<T>::iterator: public std::iterator<
  std::forward_iterator_tag,
  T*> {
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

template<typename T>
auto ParseTree<T>::begin() {
  return iterator(children_.begin());
}

template<typename T>
auto ParseTree<T>::end() {
  return iterator(children_.end());
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PARSETREE_IMPL */
