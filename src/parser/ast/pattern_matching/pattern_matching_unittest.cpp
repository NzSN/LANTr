#include <gtest/gtest.h>
#include <initializer_list>
#include <rapidcheck/gtest.h>
#include <variant>

#include "base/n_ary_tree.hpp"
#include "parser/ast/pattern_matching/pattern_matching.hpp"

namespace LANTr::Parser::AST::PatternMatch {

#define _ NumOrNums
struct NumOrNums {
  using initializer = std::variant<int, NumOrNums>;
  NumOrNums(std::initializer_list<initializer> list) {

  }
};

struct NumericTree: public Base::Tree<NumericTree> {
  NumericTree(std::initializer_list<NumOrNums> list):
    Tree(this) {

    for (const auto& nums: list) {

    }
  }
};

struct PatternMatchTester: public ::testing::Test {};

RC_GTEST_FIXTURE_PROP(PatternMatchTester, SimpleMatch, ()) {
  NumericTree tree{
    // First
    {1, _{2,3,4}, 3, _{2,3,4,5}},
    // Second
    {1, _{3,4,5}, 5, _{1,2,3,4}},
  };
}


} // LANTr::Parser::AST::PatternMatch
