#ifndef LANTR_PARSER_PARSER_H_
#define LANTR_PARSER_PARSER_H_

#include "base/tree_layer.hpp"
#include "base/types.hpp"
#include "parser/ast/antlr_parsetree.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/languages/language_impl.hpp"

#include "parser/parser_info.hpp"

namespace LANTr::Parser {

template<LANGUAGE::LANGUAGE L>
class [[nodiscard]] ParseResult {
  using Info = ParserInfo<LANGUAGE::ImplOfLang<L>::impl>;
public:
  ParseResult(LANGUAGE::ParseResult<L>&& r):
    result_{std::move(r)},
    tree_{Info::Tree::BuildFrom(result_.tree)},
    tree{tree_.get()} {}
private:
  LANGUAGE::ParseResult<L> result_;
  std::unique_ptr<typename Info::Tree> tree_;
public:
  Info::Tree *tree;
};

template<LANGUAGE::LANGUAGE L>
class Parser {
public:
  static ParseResult<L> Parse(Base::Types::Source source) {
    LANGUAGE::ParseResult<L> result =
      LANGUAGE::ParserImpl::Parse<L>(source);

    return {std::move(result)};
  }
};

} // LANTr::Parser

#endif // LANTR_PARSER_PARSER_H_
