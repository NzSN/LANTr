#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_IMPL
#define LANTR_PARSER_AST_ANTLRPARSETREE_IMPL

#include <memory>
#include "base/utilities/assert.hpp"
#include "src/parser/ast/antlr_parsetree.hpp"

namespace LANTr::Parser::AST {

using AntlrInternal = antlr4::tree::ParseTree;

std::unique_ptr<AntlrTree> AntlrTree::BuildFrom(InternalTree* tree) {
  ASSERT(tree != nullptr, "BuildFrom nullptr");

  auto root = std::make_unique<AntlrTree>(tree);
  root->children_ = Children(tree->children.size());

  if (!root->children_.empty()) {
    std::transform(tree->children.begin(), tree->children.end(),
                   root->children_.begin(),
                   [this](AntlrInternal* tree) {
                     return BuildFrom(tree);
                   });
  }

  return root;
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_ANTLRPARSETREE_IMPL */
