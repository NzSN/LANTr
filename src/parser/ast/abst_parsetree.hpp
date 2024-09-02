#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_H_
#define LANTR_PARSER_AST_ANTLRPARSETREE_H_

#include "antlr4-runtime.h"
#include "base/tree_layer.hpp"
#include "parser/ast/pattern_matching/matchable.hpp"
#include <type_traits>

namespace LANTr::Parser::AST {


template<typename T>
struct AbstTree {
  static_assert(std::integral_constant<bool, false>::value,
    "Not supported implementation");
};

using Antlr4Tree = antlr4::tree::ParseTree;
template<>
class AbstTree<Antlr4Tree>: public Base::TreeLayer<AbstTree<Antlr4Tree>, Antlr4Tree>,
                            public Matchable<AbstTree<Antlr4Tree>> {
private:
  using EqualFn = std::function<bool(const AbstTree&, const AbstTree&)>;

  bool IsNodeValueEqual(const AbstTree& tree, const AbstTree& other) const {
    return
      tree.children_.size() == other.children_.size() &&
      tree.Text() == other.Text() &&
      tree.TreeType() == other.TreeType();
  }
public:
  AbstTree(Antlr4Tree* tree): TreeLayer(this, tree) {}

  std::string Text() const {
    return lower_->getText();
  }

  size_t TreeType() const {
    return static_cast<size_t>(lower_->getTreeType());
  }

  // Implement as Pattern
  [[nodiscard]] bool operator==(const AbstTree& other) const {
    return IsNodeValueEqual(*this, other);
  }

private:
  friend struct AstTest;

  const Antlr4Tree* GetInternal() const {
    return const_cast<const Antlr4Tree*>(lower_);
  }
};

} // LANTr::Parser::AST


#endif // LANTR_PARSER_AST_ANTLRPARSETREE_H_
