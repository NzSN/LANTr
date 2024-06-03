#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>
#include <variant>

#include "n_ary_tree.hpp"
#include "tree_layer.hpp"

namespace LANTr::Base {


///////////////////////////////////////////////////////////////////////////////
//                                    Tree                                   //
///////////////////////////////////////////////////////////////////////////////
struct Node: public Tree<Node> {
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
  for (auto& i: node.GetChildren()) {
    RC_ASSERT(i->nNum == *iter++);
  }
}

///////////////////////////////////////////////////////////////////////////////
//                                 TreeLayer                                 //
///////////////////////////////////////////////////////////////////////////////
struct Lower: public Tree<Lower> {};

struct Upper: public TreeLayer<Upper,Lower> {
  Upper(): TreeLayer{nullptr} {}
  Upper(Lower* lower): TreeLayer{lower} {}
};

using Layers = std::variant<Upper, Lower>;

struct TreeLayerTester: public ::testing::Test {
  [[nodiscard]] std::unique_ptr<Lower> GenLowerLayer(int numOfNodes) {
    std::unique_ptr<Lower> root = std::make_unique<Lower>();
    --numOfNodes;
    if (numOfNodes == 0) {
      return root;
    }

    while (numOfNodes > 0) {
      int numOfChildren = *rc::gen::inRange(0, numOfNodes);
      root->AddChild(GenLowerLayer(numOfChildren));
      numOfNodes -= numOfChildren + 1;
    }

    return root;
  }

  [[nodiscard]] std::unique_ptr<Upper>
  GenConsistencyUpper(Lower* lower) {
    return Upper::BuildFrom(lower);
  }

  [[nodiscard]] std::unique_ptr<Upper>
  GenInconsistencyUpper(Lower* lower, bool hasInconsistencyPoint = false) {
    //RC_ASSERT(lower != nullptr);

    auto root = std::make_unique<Upper>(lower);
    for (auto& c: lower->GetChildren()) {
      bool inconsistencyPoint = *rc::gen::arbitrary<bool>();

      if (inconsistencyPoint ||
          (!hasInconsistencyPoint &&
           !c->HasChild())) {
        hasInconsistencyPoint = true;
        continue;
      } else {
        root->AddChild(GenInconsistencyUpper(
                         c.get(), inconsistencyPoint || hasInconsistencyPoint));
      }
    }

    return root;
  }

  template<bool IsConsistency>
  std::vector<std::unique_ptr<Upper>>
  GetLayers(int numOfLayers) {
    std::vector<std::unique_ptr<Upper>> layers;

    while (numOfLayers > 0) {
      // Generate lower layer
      int numOfNodes = 100;
      std::unique_ptr<Lower> lowerLayer = GenLowerLayer(numOfNodes);

      // Generate consistency upper layer
      std::unique_ptr<Upper> upperLayer =
        IsConsistency ? GenConsistencyUpper(lowerLayer.get())
                      : GenInconsistencyUpper(lowerLayer.get());

      layers.push_back(std::move(upperLayer));
      lowers.push_back(std::move(lowerLayer));

      --numOfLayers;
    }

    return layers;
  }

  std::vector<std::unique_ptr<Lower>> lowers;
};

RC_GTEST_FIXTURE_PROP(TreeLayerTester, LayerConsistentcy, ()) {
  constexpr int numOfLayers = 100;
  for (auto& layer: GetLayers<true>(numOfLayers)) {
    RC_ASSERT(layer->IsLayerEquivalent());
  }

  for (auto& layer: GetLayers<false>(numOfLayers)) {
    RC_ASSERT(!layer->IsLayerEquivalent());
  }
}

RC_GTEST_FIXTURE_PROP(TreeLayerTester, LayerSynchronization, ()) {

}

} // LANTr::Base
