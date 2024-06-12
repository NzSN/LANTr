#ifndef LANTR_PARSER_PARSER_H_
#define LANTR_PARSER_PARSER_H_

#include "base/types.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/languages/language_impl.hpp"

#include "parser/parser_info.hpp"

namespace LANTr::Parser {

template<LANGUAGE::LANGUAGE L>
class [[nodiscard]] ParseResult {
public:


private:
  using ParseResult_Unique =
    std::unique_ptr<LANGUAGE::ParseResult<L>>;

  ParseResult_Unique result;


};

template<LANGUAGE::LANGUAGE L>
class Parser {
public:
  ParseResult<L> Parse(Base::Types::Source source);
};

} // LANTr::Parser

#endif // LANTR_PARSER_PARSER_H_
