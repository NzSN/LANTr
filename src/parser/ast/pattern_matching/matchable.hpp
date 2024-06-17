#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_

#include "base/tree_concepts.hpp"
#include <concepts>
#include <type_traits>

namespace LANTr::Parser::AST {

template<typename T>
concept is_matchable = requires(T& t, T& o) {
  { t.match(o) } -> std::same_as<bool>;
};

template<typename T>
class Matchable {
public:
  Matchable() {
    static_assert(Base::TreeConcepts::NAryTree<T>);
  }

  bool match(Matchable& other) {

  }
};

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_ */
