#ifndef LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_
#define LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_

#include <concepts>
#include <functional>
#include <optional>
#include <type_traits>

namespace LANTr::Parser::AST {

template<typename T>
concept IsMatchable = requires(T& t, T& o) {
  { t.IsVar() } -> std::same_as<bool>;
  { t.operator==(o) } -> std::same_as<bool>;
  { t.bound() } -> std::same_as<T*>;
};

// TODO: Rename Matchable to SigmaTerm is more apropriate.
template<typename T>
class Matchable {
public:
  using Matcher = std::function<bool(const Matchable, const Matchable)>;

  Matchable(bool is_var = false): is_var_{is_var} {
    static_assert(IsMatchable<T>);
    static_assert(std::derived_from<T, Matchable<T>>);
  }

  [[nodiscard]] bool IsVar() const {
    return is_var_;
  }

  void AsVar() {
    is_var_ = true;
  }

  void AsNonVar() {
    is_var_ = false;
  }

  [[nodiscard]] T* bound() {
    return bound_;
  }

  void SetBound(T* bound) {
    bound_ = bound;
  }

  [[nodiscard]]
  bool operator==(const Matchable& other) const {
    return static_cast<const T*>(this)->operator==(
      static_cast<const T&>(other))
      /* Term Variable works as wildcard */
      || is_var_
      || other.is_var_;
  }

private:
  bool is_var_;
  T* bound_;
};

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_ */
