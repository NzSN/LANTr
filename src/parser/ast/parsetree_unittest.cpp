#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "parsetree-inl.hpp"

namespace LANTr::Parser::AST {

struct Node: public ParseTree<Node> {
  Node(int num): nNum(num) {}

  void AddChildren(std::vector<int> nums) {
    std::for_each(nums.begin(), nums.end(),
                  [this](int num) {
                    auto node = std::make_unique<Node>(num);
                    this->children_.push_back(std::move(node));
                  });
  }

  int nNum;
};

RC_GTEST_PROP(ParseTreeTest, ParseTreeNav, ()) {
  Node node(1);

  // Generate vector of int randomly
  std::vector<int> nums;
  int numOfChildren = *rc::gen::inRange(0, 100);
  while (numOfChildren > 0) {
    nums.push_back(*rc::gen::inRange(0,100));
    --numOfChildren;
  }

  node.AddChildren(nums);
  auto iter = nums.begin();
  for (auto i: node) {
    RC_ASSERT(i->nNum == *iter++);
  }
}


} // LANTr::Parser::AST
