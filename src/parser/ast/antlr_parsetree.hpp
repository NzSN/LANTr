#ifndef LANTR_PARSER_AST_ANTLRPARSETREE
#define LANTR_PARSER_AST_ANTLRPARSETREE

#include "antlr4-runtime.h"
#include "parsetree-inl.hpp"

namespace LANTr::Parser::AST {

using InternalTree = antlr4::tree::ParseTree;

class AntlrTree: public ParseTree<AntlrTree> {
public:
  AntlrTree(InternalTree* tree): node_(tree) {}
  std::unique_ptr<AntlrTree> BuildFrom(InternalTree* tree);
private:
  InternalTree* node_;
};

} // LANTr::Parser::AST


#endif /* LANTR_PARSER_AST_ANTLRPARSETREE */
