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

template<typename T>
class Matchable {
public:
  using Matcher = std::function<bool(const Matchable, const Matchable)>;

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

  [[nodiscard]]
  bool Equal(const Matchable& lhs, const Matchable& rhs) const {

  }

private:
  bool is_var_;
  T* bound_;
};

} // LANTr::Parser::AST

#endif /* LANTR_PARSER_AST_PATTERN_MATCHING_MATCHABLE_H_ */
