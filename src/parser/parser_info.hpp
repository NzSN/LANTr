#ifndef LANGR_PARSER_PARSER_INFO_H_
#define LANGR_PARSER_PARSER_INFO_H_

#include "parser/ast/abst_parsetree.hpp"
#include "parser/languages/language_definitions.hpp"

namespace LANTr::Parser {

template<LANGUAGE::LANG_IMPL LI>
struct ParserInfo;

template<>
struct ParserInfo<LANGUAGE::ANTLR4> {
  using Tree = AST::AbstTree<
    LANGUAGE::TreeType<LANGUAGE::ANTLR4>::type>;
};

} // LANTr::PARSER


#endif /* LANGR_PARSER_PARSER_INFO_H_ */
