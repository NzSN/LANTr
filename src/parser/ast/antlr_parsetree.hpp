#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_H_
#define LANTR_PARSER_AST_ANTLRPARSETREE_H_

#include "antlr4-runtime.h"
#include "base/utilities/assert.hpp"
#include "base/n_ary_tree.hpp"

namespace LANTr::Parser::AST {

using InternalTree = antlr4::tree::ParseTree;

class AntlrTree: public Base::Tree<AntlrTree> {
public:
  AntlrTree(InternalTree* tree): node_(tree) {
    ASSERT(node_, "AntlrTree is initialized from nullptr");
  }
  static std::unique_ptr<AntlrTree> BuildFrom(InternalTree* tree);

  std::string Text() const {
    return node_->getText();
  }

private:
  friend struct AstTest;

  const InternalTree* GetInternal() const {
    return const_cast<const InternalTree*>(node_);
  }

  InternalTree* node_;
};

} // LANTr::Parser::AST


#endif // LANTR_PARSER_AST_ANTLRPARSETREE_H_
