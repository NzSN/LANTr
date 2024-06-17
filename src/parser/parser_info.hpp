#ifndef LANGR_PARSER_PARSER_INFO_H_
#define LANGR_PARSER_PARSER_INFO_H_

#include "parser/ast/antlr_parsetree.hpp"
#include "parser/languages/language_definitions.hpp"

namespace LANTr::Parser {

template<LANGUAGE::LANG_IMPL LI>
struct ParserInfo;

template<>
struct ParserInfo<LANGUAGE::ANTLR4> {
  using Tree = AST::AntlrTree;
};

} // LANTr::PARSER


#endif /* LANGR_PARSER_PARSER_INFO_H_ */
