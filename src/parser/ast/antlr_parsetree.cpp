#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_IMPL
#define LANTR_PARSER_AST_ANTLRPARSETREE_IMPL

#include "antlr_parsetree.hpp"

namespace LANTr::Parser::AST {

[[nodiscard]] bool AntlrTree::operator==(const AntlrTree& other) const {
  auto iter_l = this->begin();
  auto iter_r = other.begin();

  for (; iter_l != this->end() &&
         iter_r != other.end(); ++iter_l, ++iter_r) {
    if (iter_l->children_.size() != iter_r->children_.size() ||
        iter_l->Text() != iter_r->Text() ||
        iter_l->TreeType() != iter_r->TreeType()) {
      return false;
    }
  }

  if (iter_l == this->end() && iter_r == other.end()) {
    return true;
  } else {
    return false;
  }
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_ANTLRPARSETREE_IMPL */
