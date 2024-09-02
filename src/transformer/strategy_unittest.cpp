#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "parser/languages/language_definitions.hpp"
#include "strategy.hpp"
#include "transformer/rule.hpp"

namespace LANTr::Transformer {

TEST(StrategyTester, Spec) {
  Rule<Parser::LANGUAGE::ARITH> rule{"1+1", "2+2"};
  MatchStra<Parser::LANGUAGE::ARITH> matcher{rule};

  //matcher("1+1");

}


} // LANTr::Transformer
