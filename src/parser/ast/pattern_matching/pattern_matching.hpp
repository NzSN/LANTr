#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_

#include <algorithm>

#include "parser/ast/pattern_matching/matchable.hpp"
#include "base/tree_concepts.hpp"

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
using MatchResult = std::vector<Matchable<T>>;

namespace {

template<MatchAlgo Algo, typename T>
requires (Algo == NAIVE)
void Stepping(MatchResult<T>& result, const T& tree, const T& pattern) {
  for (auto current = tree.begin();
       current != tree.end(); ++current) {


  }
}

}

template<MatchAlgo Algo = NAIVE, typename T>
requires std::derived_from<T, Matchable<T>> &&
         Base::TreeConcepts::NAryTree<T> &&
         (Algo == NAIVE)
[[nodiscard]]
std::vector<Matchable<T>>
Matching(const T* tree,
         const T* pattern) {
  ASSERT(tree && pattern);

  std::vector<Matchable<T>> matchs;

  std::for_each(tree->begin(), tree->end(),
                [&](T& t) {
                  Stepping<NAIVE,T>(matchs, t, *pattern);
                });

  return matchs;
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_ */
