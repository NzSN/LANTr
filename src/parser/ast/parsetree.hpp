#ifndef LANTR_PARSER_AST_PARSERTREE
#define LANTR_PARSER_AST_PARSERTREE

#include <memory>
#include <vector>

namespace LANTr::Parser::AST {

template<typename T>
class ParseTree {
public:
  using Children = std::vector<std::unique_ptr<T>>;

  // Iterator
  class iterator;
  auto begin();
  auto end();

  const T* Parent() const {
    return parent_;
  }

  void SetParent(T* parent) {
    parent_ = parent;
  }

  void AddChild(std::unique_ptr<T> child) {
    children_.push_back(std::move(child));
  }

protected:
  T* parent_;
  Children children_;
};

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PARSERTREE */
