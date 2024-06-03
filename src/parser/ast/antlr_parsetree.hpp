#ifndef LANTR_PARSER_AST_ANTLRPARSETREE_H_
#define LANTR_PARSER_AST_ANTLRPARSETREE_H_

#include "antlr4-runtime.h"
#include "base/tree_layer.hpp"

namespace LANTr::Parser::AST {

using InternalTree = antlr4::tree::ParseTree;

class AntlrTree: public Base::TreeLayer<AntlrTree, InternalTree> {
public:
  AntlrTree(InternalTree* tree): TreeLayer(this, tree) {}

  std::string Text() const {
    return lower_->getText();
  }

private:
  friend struct AstTest;

  const InternalTree* GetInternal() const {
    return const_cast<const InternalTree*>(lower_);
  }
};

} // LANTr::Parser::AST


#endif // LANTR_PARSER_AST_ANTLRPARSETREE_H_
