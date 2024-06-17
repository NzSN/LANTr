#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_IMPL
#define LANTR_PARSER_AST_ANTLRPARSETREE_IMPL

#include "antlr_parsetree.hpp"

namespace LANTr::Parser::AST {

bool AntlrTree::IsNodeValueEqual(
  const AntlrTree& tree,
  const AntlrTree& other) const {

  return
    tree.children_.size() == other.children_.size() &&
    tree.Text() == other.Text() &&
    tree.TreeType() == other.TreeType();
}

[[nodiscard]] bool AntlrTree::operator==(const AntlrTree& other) const {
  return IsNodeValueEqual(*this, other);
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_ANTLRPARSETREE_IMPL */
