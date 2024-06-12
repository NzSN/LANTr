#include <gtest/gtest.h>
#include <optional>
#include <rapidcheck/gtest.h>

#include <variant>
#include <algorithm>

#include "base/utilities/assert.hpp"
#include "test_utility.hpp"
#include "base/utilities/bottom.hpp"
#include "base/n_ary_tree.hpp"
#include "base/tree_layer.hpp"

namespace LANTr::Base {


///////////////////////////////////////////////////////////////////////////////
//                                    Tree                                   //
///////////////////////////////////////////////////////////////////////////////
struct Node: public Tree<Node> {
  Node(int num): Tree<Node>{this}, nNum(num) {}

  void AddChildren(std::vector<int>& nums) {
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

RC_GTEST_PROP(NARYTreeTest, Traversal_Base, ()) {
  Node node(1);

  int count{};
  for (auto& iter: node) {
    RC_ASSERT(iter.nNum == 1);
    ++count;
  }

  RC_ASSERT(count == 1);
}

RC_GTEST_PROP(NAryTreeTest, Traversal, ()) {
  Node node(1);

  std::vector<int> nums{1,23,4,5};
  node.AddChildren(nums);
  auto iter = node.GetChildren().begin();;
  (*iter)->AddChildren(nums);

  std::vector<int> expected{1, 1, 1, 23, 4, 5, 23,4,5};
  auto iter_expected = expected.begin();
  for (auto& iter: node) {
    RC_ASSERT(iter_expected != expected.end());
    RC_ASSERT(*iter_expected == iter.nNum);

    ++iter_expected;
  }
}

///////////////////////////////////////////////////////////////////////////////
//                                 TreeLayer                                 //
///////////////////////////////////////////////////////////////////////////////
struct Lower: public Tree<Lower> {
  Lower(): Tree<Lower>(this) {}
};

struct Upper: public TreeLayer<Upper,Lower> {
  Upper(): TreeLayer{this, nullptr} {}
  Upper(Lower* lower): TreeLayer{this, lower} {}
};

using Layers = std::variant<Upper, Lower>;

struct TreeLayerTester: public ::testing::Test {

  std::optional<std::tuple<Upper*, Upper*>>
  SwapLower(Upper* upper, int indexL, int indexR) {
    Upper* from{};
    Upper* to{};

    int count = 0;
    for (auto& node: *upper) {
        if (count == indexL || count == indexR) {
          if (from == nullptr) {
            ASSERT(to == nullptr, "'to' should not be setted before 'from'");
            from = &node;
          } else {
            ASSERT(from != nullptr, "'to' should not be setted before 'from'");
            to = &node;

            std::swap(from->lower_, to->lower_);

            return std::make_tuple(from, to);
          }
        }
        ++count;
    }
    return std::nullopt;
  }

  std::optional<std::tuple<Upper*, Upper*>>
  InvalidateStep(Upper* upper,
    std::vector<int>& invalidateds) {

    int numOfNodes = GetNumOfNodes(*upper);
    int nthToSelect_1st = RC::RandomNumUntil(
      0, numOfNodes,
      [&](int num) -> bool {
        return num !=0
          && (num % numOfNodes) != 0
          && std::find(invalidateds.begin(),
                       invalidateds.end(),
                       num)
             == invalidateds.end();
      });
    int nthToSelect_2nd = RC::RandomNumUntil(
      0, numOfNodes,
      [&](int num) -> bool {
        return num != nthToSelect_1st
          && (num % numOfNodes) != 0
          && std::find(invalidateds.begin(),
                       invalidateds.end(),
                       num)
             == invalidateds.end();
      });

    invalidateds.push_back(nthToSelect_1st);
    invalidateds.push_back(nthToSelect_2nd);

    return SwapLower(upper, nthToSelect_1st, nthToSelect_2nd);
  }

  std::vector<Upper*> LetSomeNodesToBeInvalidated(Upper* upper) {
    std::vector<Upper*> invalidatedNodes;
    std::vector<int> records;

    int numToInvalidate =
      *rc::gen::inRange(0, GetNumOfNodes(*upper)) / 2;

    while (numToInvalidate > 0) {
      std::optional<std::tuple<Upper*, Upper*>> invalids_opt =
        InvalidateStep(upper, records);
      if (!invalids_opt.has_value()) {
        continue;
      }

      auto invalids = invalids_opt.value();
      ASSERT(std::get<0>(invalids) != nullptr &&
             std::get<1>(invalids) != nullptr,
             "invalidate a nullptr is impossible");
      invalidatedNodes.push_back(std::get<0>(invalids));
      invalidatedNodes.push_back(std::get<1>(invalids));

      --numToInvalidate;
    }

    std::sort(invalidatedNodes.begin(), invalidatedNodes.end());
    invalidatedNodes.erase(std::unique(invalidatedNodes.begin(), invalidatedNodes.end()),
                           invalidatedNodes.end());

    // Kick out children of rest of nodes and duplicates
    std::vector<Upper*> processed;

    std::for_each(
      invalidatedNodes.begin(), invalidatedNodes.end(),
      [&](Upper* upper) {
        bool isDescdent{false};
        std::for_each(invalidatedNodes.begin(), invalidatedNodes.end(),
                      [&](Upper* upper_inner) {
                        isDescdent =
                          isDescdent || upper->IsDescdentOf(*upper_inner);
                      });
        if (!isDescdent) {
          processed.push_back(upper);
        }
      });

    return processed;
  }

  std::vector<Upper*> PartialSomeNodes(Upper* upper) {
    std::vector<Upper*> partialedNodes{};
    std::for_each(upper->begin(), upper->end(),
                  [&](Upper& upper_) {
                    bool doPartial = *rc::Arbitrary<bool>::arbitrary();
                    if (doPartial && !upper_.children_.empty()) {
                      // This operation does not invalidate the
                      // 'upper' iterator.
                      upper_.children_.pop_back();
                      partialedNodes.push_back(&upper_);
                    }
                  });
    return partialedNodes;
  }

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
    RC_ASSERT(lower != nullptr);

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
  std::unique_ptr<Upper>
  GetLayers() {
    // Generate lower layer
    int numOfNodes = 100;
    std::unique_ptr<Lower> lowerLayer = GenLowerLayer(numOfNodes);

    // Generate consistency upper layer
    std::unique_ptr<Upper> upperLayer =
      IsConsistency ? GenConsistencyUpper(lowerLayer.get())
                    : GenInconsistencyUpper(lowerLayer.get());

    lowers.push_back(std::move(lowerLayer));

    return upperLayer;
  }

  void CheckInvalidated(Upper* upper) {
    upper->UpdateInvalidateState();
  }

  bool IsInvalidNode(Upper* upper) {
    return upper->state_ == TreeLayer<Upper, Lower>::INVALID;
  }

  bool IsPartialNode(Upper* upper) {
    return upper->state_ == TreeLayer<Upper, Lower>::PARTIALLY_VALID;
  }


  std::vector<std::unique_ptr<Lower>> lowers;
};

RC_GTEST_FIXTURE_PROP(TreeLayerTester, LayerConsistentcy, ()) {
  RC_ASSERT(GetLayers<true>()->IsLayerEquivalent());
  RC_ASSERT(!GetLayers<false>()->IsLayerEquivalent());
}

RC_GTEST_FIXTURE_PROP(TreeLayerTester, Invalidate, ()) {
  auto layer = GetLayers<true>();
  std::vector<Upper*> invalidatedNodes = LetSomeNodesToBeInvalidated(layer.get());

  CheckInvalidated(layer.get());

  // Then iterative over the tree to collect all
  // invalidated nodes.
  std::vector<Upper*> invalidatedFounds;
  std::for_each(layer->begin(), layer->end(),
                [&](auto& node) {
                  if (IsInvalidNode(&node)) {
                    invalidatedFounds.push_back(&node);
                  }
                });

  RC_ASSERT(invalidatedNodes.size() == invalidatedFounds.size());

  // Those nodes been collected should be found in 'invalidatedNodes'
  std::for_each(invalidatedFounds.begin(),
                invalidatedFounds.end(),
                [&](const Upper* node) {
                  Upper* parent = node->Parent();
                  while (parent) {
                    RC_ASSERT(IsPartialNode(parent));
                    parent = parent->Parent();
                  }

                  RC_ASSERT(std::find(invalidatedNodes.begin(),
                            invalidatedNodes.end(), node) !=
                            invalidatedNodes.end());
                });
}

RC_GTEST_FIXTURE_PROP(TreeLayerTester, PartialCheck, ()) {
  auto layer = GetLayers<true>();

  std::vector<Upper*> partials = PartialSomeNodes(layer.get());
  CheckInvalidated(layer.get());

  std::for_each(partials.begin(), partials.end(),
                [&](Upper* u) {
                  RC_ASSERT(IsPartialNode(u));
                });

  // Due to we don't alter relations except
  // partial so there must no INVAID state nodes.
  std::for_each(partials.begin(), partials.end(),
                [&](Upper* u) {
                  RC_ASSERT(!IsInvalidNode(u));
                });
}

RC_GTEST_FIXTURE_PROP(TreeLayerTester, LayerSynchronization, ()) {
  auto layer = GetLayers<false>();
  RC_ASSERT(!layer->IsLayerEquivalent());
  layer->Synchronize();
  RC_ASSERT(layer->IsLayerEquivalent());
}

} // LANTr::Base
