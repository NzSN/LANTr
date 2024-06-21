#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_

#include <algorithm>

#include "parser/ast/pattern_matching/matchable.hpp"

namespace LANTr::Parser::AST::PatternMatch {

template<typename T>
requires std::derived_from<T, Matchable<T>> &&
         Base::TreeConcepts::NAryTree<T>
[[nodiscard]]
std::vector<Matchable<T>>
NaiveMatch(const T* tree,
           const T* pattern) {
  ASSERT(tree && pattern);

  std::vector<Matchable<T>> matchs;

  std::for_each(
    tree->begin(), tree->end(),
    [&](T& t) {

    });

  return matchs;
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_PATTERN_MATCHING_H_ */
