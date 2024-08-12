#ifndef LANTR_PARSER_LANGUAGES_H_
#define LANTR_PARSER_LANGUAGES_H_

#include <memory>
#include <sstream>
#include <stdexcept>

#include "antlr4-runtime.h"

#include "base/types.hpp"
#include "language_definitions.hpp"

namespace LANTr::Parser::LANGUAGE {

template<LANGUAGE L>
struct ParseResult;

template<LANGUAGE L>
struct ParseResult {
  ParseResult(Base::Types::Source source):
    input_stream_{source},
    input_{std::make_unique<antlr4::ANTLRInputStream>(input_stream_)},
    lexer_{std::make_unique<typename LangLexer<L>::type>(input_.get())},
    tokens_{std::make_unique<antlr4::CommonTokenStream>(lexer_.get())},
    parser_{std::make_unique<typename LangParser<L>::type>(tokens_.get())},
    tree{Antlr4Entry<L>::GetEntry(*parser_)}
    {}

  bool HasErrors() const {
    return parser_->getNumberOfSyntaxErrors() > 0;
  }

private:
  // Caution: following resources is required to be lived otherwise
  //          tree is deaded.
  std::istringstream input_stream_;
  std::unique_ptr<antlr4::ANTLRInputStream> input_;
  std::unique_ptr<typename LangLexer<L>::type> lexer_;
  std::unique_ptr<antlr4::CommonTokenStream> tokens_;
  std::unique_ptr<typename LangParser<L>::type> parser_;
public:
  TreeType<ANTLR4>::type* tree;
};

class ParserImpl {
public:
  template<LANGUAGE L>
  requires CanParsedBy<L, ANTLR4>
  static ParseResult<L> Parse(Base::Types::Source source);
};

template<LANGUAGE L>
requires CanParsedBy<L, ANTLR4>
ParseResult<L>
ParserImpl::Parse(Base::Types::Source source) {
  std::istringstream iss{source};
  return ParseResult<L>{source};
}

} // LANTr::Parser::LANGUAGE


#endif // LANTR_PARSER_LANGUAGES_H_
