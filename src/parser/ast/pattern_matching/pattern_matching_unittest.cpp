#include <gtest/gtest.h>
#include <initializer_list>
#include <rapidcheck/gtest.h>
#include <type_traits>
#include <utility>
#include <variant>

#include "base/n_ary_tree.hpp"
#include "base/utilities/bottom.hpp"

#include "parser/ast/pattern_matching/pattern_matching.hpp"


namespace LANTr::Parser::AST::PatternMatch {

#define _ NumOrNums
struct NumOrNums {
  using initializer = std::variant<int, NumOrNums>;

  NumOrNums() = default;
  NumOrNums(std::initializer_list<initializer> list) {
    for (auto l: list) {
      initializers.push_back(l);
    }
  }
  NumOrNums(std::vector<initializer> nums) {
    for (auto l: nums) {
      initializers.push_back(l);
    }
  }

  operator std::vector<NumOrNums>() const {
    std::vector<NumOrNums> numOrNums_vec(initializers.size());
    std::transform(
      initializers.begin(), initializers.end(),
      numOrNums_vec.begin(),
      [&](const initializer& i) -> NumOrNums {
        if (std::get_if<int>(&i) != nullptr) {
          return NumOrNums{{std::get<int>(i)}};
        } else if (std::get_if<NumOrNums>(&i) != nullptr) {
          return NumOrNums{std::get<NumOrNums>(i).initializers};
        } else {
          std::unreachable();
        }
      });

    return numOrNums_vec;
  }

  std::vector<initializer> initializers;
};

struct NumericTree: public Base::Tree<NumericTree>,
                    public Matchable<NumericTree> {
  NumericTree(): Tree(this), numericNode{false}, num{} {}
  NumericTree(int num_): Tree(this), numericNode{true}, num{num_} {
    if (num_ == 0) {
      AsVar();
    } else {
      AsNonVar();
    }
  }

  NumericTree(std::vector<NumOrNums> list):
    Tree(this), numericNode(false), num{} {

    for (auto& numOrNums: list) {
      auto& child = AddChild(std::make_unique<NumericTree>());

      for (auto& nums: numOrNums.initializers) {
        std::visit([&](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, int>) {
            child->AddChild(std::make_unique<NumericTree>(arg));
          } else if constexpr (std::is_same_v<T, NumOrNums>) {
            child->AddChild(std::make_unique<NumericTree>(
                              static_cast<std::vector<NumOrNums>>(arg)));
          }
        }, nums);
      }
    }

  }

  NumericTree(std::initializer_list<NumOrNums> list):
    NumericTree{std::vector<NumOrNums>{list}} {}

  bool operator==(const NumericTree& other) const {
    return num == other.num;
  }

  const bool numericNode;
  const int num;
};

struct PatternMatchTester: public ::testing::Test {};

RC_GTEST_FIXTURE_PROP(PatternMatchTester, SimpleMatch, ()) {
  NumericTree
    tree_1 { {1,2,3}, {3,2,1} },
    tree_2 { {1,2,3}, {2,1,3} },
    tree_3 { {1,2,3}, {3,2,1} };

  MatchResult<NumericTree> r1 = Matching(&tree_1, &tree_2);
  RC_ASSERT(r1.size() == 0);

  MatchResult<NumericTree> r2 = Matching(&tree_1, &tree_3);
  RC_ASSERT(r2.size() == 1);
}

RC_GTEST_FIXTURE_PROP(PatternMatchTester, MatchWithVar, ()) {
  NumericTree
    tree_1 { {1}, {2} },
    tree_2 { {0}, {0} } /* 0 treat as Term Variable */;

  MatchResult<NumericTree> r1 = Matching(&tree_1, &tree_2);
  RC_ASSERT(r1.size() == 1);
}


} // LANTr::Parser::AST::PatternMatch
