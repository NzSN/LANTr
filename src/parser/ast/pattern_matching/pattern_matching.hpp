#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_

#include <algorithm>
#include <concepts>

#include "parser/ast/pattern_matching/matchable.hpp"
#include "base/tree_concepts.hpp"
#include "base/utilities/assert.hpp"

namespace LANTr::Parser::AST::PatternMatch {

enum MatchAlgo {
  FIRST_MATCH_ALGO,
  NAIVE,
  END_MATCH_ALGO,
};

///////////////////////////////////////////////////////////////////////////////
//                           Naive Pattern Matching                          //
///////////////////////////////////////////////////////////////////////////////
template<typename T>
using MatchResult = std::vector<const T*>;

namespace {

template<MatchAlgo Algo, typename T>
requires std::derived_from<T, Matchable<T>> &&
         Base::TreeConcepts::NAryTree<T> &&
         Base::TreeConcepts::NAryTreeIterator<typename T::iterator> &&
         (Algo == NAIVE)
void Stepping(MatchResult<T>& result, const T& tree, const T& pattern) {
  auto current = tree.begin();
  auto current_pattern = pattern.begin();

  auto next_iter = [&](bool isTermVar) {
    if (isTermVar) {
      current.SkipSubTree();
    } else {
      ++current;
    }

    ++current_pattern;
  };

  while (current_pattern != pattern.end()) {

    bool isVar = static_cast<Matchable<T>*>(
      &(*current_pattern))->IsVar();

    if (current == tree.end() ||
        (!isVar &&
         Base::TreeConcepts::NumOfChildren(&(*current_pattern)) !=
         Base::TreeConcepts::NumOfChildren(&(*current)))) {
      return;
    }

    if (!current->Matchable<T>::operator==(
          *current_pattern)) {
      return;
    }

    next_iter(isVar);
  }

  if (current_pattern == pattern.end() &&
      current == tree.end())
    result.push_back(&tree);
}

}

// TODO: The type of second argument should be something like
//       'Pattern<T>' to identifying argument as Pattern explicit in
//       type system.
//
//       Type of Matching should be:
//       Matching :: T -> Pattern<T> -> Bool
template<MatchAlgo Algo = NAIVE, typename T>
requires std::derived_from<T, Matchable<T>> &&
         Base::TreeConcepts::NAryTree<T> &&
         Base::TreeConcepts::NAryTreeIterator<typename T::iterator> &&
         (Algo == NAIVE)
[[nodiscard]]
MatchResult<T>
Matching(const T* tree,
         const T* pattern) {
  ASSERT(tree && pattern);

  MatchResult<T> matchs;

  std::for_each(tree->begin(), tree->end(),
                [&](T& t) {
                  Stepping<NAIVE,T>(matchs, t, *pattern);
                });

  return matchs;
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_ */
