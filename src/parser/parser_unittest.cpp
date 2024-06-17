#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "parser/ast/pattern_matching/pattern_matching.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/parser.hpp"

namespace LANTr::Parser {

namespace PM = AST::PatternMatch;

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

  auto matchs = PM::Matching(result.tree, result.tree);
  RC_ASSERT(matchs.size() > 0);
}

} // LANTr::Parser
