#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_IMPL
#define LANTR_PARSER_AST_ANTLRPARSETREE_IMPL

#include <memory>
#include "base/utilities/assert.hpp"
#include "antlr_parsetree.hpp"

namespace LANTr::Parser::AST {

std::unique_ptr<AntlrTree> AntlrTree::BuildFrom(InternalTree* tree) {
  ASSERT(tree != nullptr, "BuildFrom nullptr");

  auto root = std::make_unique<AntlrTree>(tree);
  for (auto c: tree->children) {
    root->AddChild(BuildFrom(c));
  }

  return root;
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_ANTLRPARSETREE_IMPL */
