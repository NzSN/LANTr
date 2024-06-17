#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "parser/ast/pattern_matching/matchable.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/parser.hpp"

namespace LANTr::Parser {

RC_GTEST_PROP(ParserTester, Parse, ()) {
  ParseResult<LANGUAGE::ARITH> result =
    Parser<LANGUAGE::ARITH>::Parse("1+1");

  RC_ASSERT(result.tree);
  RC_ASSERT(result.tree->Text() == "1+1");
}

///////////////////////////////////////////////////////////////////////////////
//                        Testing with PatternMatching                       //
///////////////////////////////////////////////////////////////////////////////
RC_GTEST_PROP(PatternMatchTester, PatternMatching, ()) {
  ParseResult<LANGUAGE::ARITH> result =
    Parser<LANGUAGE::ARITH>::Parse("1+1");

  auto matchs = AST::PatternMatching(result.tree, result.tree);
  RC_ASSERT(matchs.size() > 0);
}

} // LANTr::Parser
