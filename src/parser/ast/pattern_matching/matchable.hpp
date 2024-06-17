#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_

#include <concepts>
#include <algorithm>

#include "base/tree_concepts.hpp"
#include "base/utilities/assert.hpp"

namespace LANTr::Parser::AST {

template<typename T>
concept IsMatchable = requires(T& t, T& o) {
  { t.IsVar() } -> std::same_as<bool>;
  { t.operator==(o) } -> std::same_as<bool>;
  { t.bound() } -> std::same_as<T*>;
};

template<typename T>
class Matchable {
public:
  Matchable(bool is_var = false): is_var_{is_var} {
    static_assert(IsMatchable<T>);
  }

  [[nodiscard]] bool IsVar() const {
    return is_var_;
  }

  [[nodiscard]] T* bound() {
    return bound_;
  }

  void SetBound(T* bound) {
    bound_ = bound;
  }

private:
  bool is_var_;
  T* bound_;
};

template<typename T>
requires std::derived_from<T, Matchable<T>> &&
         Base::TreeConcepts::NAryTree<T>
[[nodiscard]]
std::vector<Matchable<T>>
PatternMatching(const T* tree,
                const T* pattern) {
  ASSERT(tree && pattern);

  std::vector<Matchable<T>> matchs;

  std::for_each(tree->begin(), tree->end(),
                [&](T& t) {

                });

  return matchs;
}

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_ */
