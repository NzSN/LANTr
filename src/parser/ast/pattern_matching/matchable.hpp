#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_

#include "base/tree_concepts.hpp"
#include <concepts>
#include <type_traits>

namespace LANTr::Parser::AST {

template<typename T>
concept IsPattern = requires(T& t, T& o) {
  { t.IsVar() } -> std::same_as<bool>;
  { t.operator==(o) } -> std::same_as<bool>;
  { t.bound() } -> std::same_as<T*>;
};

template<typename T>
class Pattern {
public:
  Pattern(bool is_var = false): is_var_{is_var} {
    static_assert(IsPattern<T>);
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
requires std::derived_from<T, Pattern<T>>
[[nodiscard]]
std::vector<Pattern<T>*>
PatternMatching(T* tree, Pattern<T>* pattern) {

}


} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_ */
