#ifndef LANTR_PARSER_LANGUAGES_H_
#define LANTR_PARSER_LANGUAGES_H_

#include <memory>

#include "antlr4-runtime.h"

#include "base/types.hpp"
#include "language_definitions.hpp"

namespace LANTr::Parser::LANGUAGE {

template<LANGUAGE L>
struct ParseResult;

template<LANGUAGE L>
struct ParseResult {
  TreeType<ANTLR4>::type* tree;

  // Caution: following resources is required to be lived otherwise
  //          tree is deaded.
  std::unique_ptr<antlr4::ANTLRInputStream> input;
  std::unique_ptr<antlr4::CommonTokenStream> tokens;
  std::unique_ptr<typename LangLexer<L>::type> lexer;
  std::unique_ptr<typename LangParser<L>::type> parser;
};

class ParserImpl {
public:
  template<LANGUAGE L>
  requires CanParsedBy<L, ANTLR4>
  ParseResult<L> Parse(Base::Types::Source source);
};

template<LANGUAGE L>
requires CanParsedBy<L, ANTLR4>
ParseResult<L>
ParserImpl::Parse(Base::Types::Source source) {
  std::istringstream iss{source};

  ParseResult<L> result;
  result.input = std::make_unique<antlr4::ANTLRInputStream>(iss);
  result.lexer = std::make_unique<typename LangLexer<L>::type>(
    result.input.get());
  result.tokens = std::make_unique<antlr4::CommonTokenStream>(
    result.lexer.get());
  result.parser = std::make_unique<typename LangParser<L>::type>(
    result.tokens.get());
  result.tree = Antlr4Entry<L>::GetEntry(*result.parser);

  return result;
}


} // LANTr::Parser::LANGUAGE


#endif // LANTR_PARSER_LANGUAGES_H_
