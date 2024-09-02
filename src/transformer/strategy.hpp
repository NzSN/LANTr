#ifndef LANTR_TRANSFORMER_STRATEGY_H_
#define LANTR_TRANSFORMER_STRATEGY_H_

#include "base/types.hpp"
#include "parser/ast/abst_parsetree.hpp"
#include "parser/ast/pattern_matching/pattern_matching.hpp"
#include "rule.hpp"
#include "parser/languages/language_definitions.hpp"
#include <algorithm>
#include <initializer_list>
#include <utility>

namespace P = ::LANTr::Parser;
namespace LANGS = P::LANGUAGE;
namespace AST = P::AST;
namespace PM = P::AST::PatternMatch;
namespace Types = ::LANTr::Base::Types;

namespace LANTr::Transformer {

template<LANGS::LANGUAGE lang>
using LangImpl = LANGS::ImplOfLang<lang>;
template<LANGS::LANGUAGE lang>
using TreeType = LANGS::TreeType<LangImpl<lang>::impl>;
template<LANGS::LANGUAGE lang>
using Tree     = typename TreeType<lang>::type;
template<LANGS::LANGUAGE lang>
using AbstTree = AST::AbstTree<Tree<lang>>;

template<typename T>
struct LangOfStrategy;
template<template<LANGS::LANGUAGE L> class T, LANGS::LANGUAGE L>
struct LangOfStrategy<T<L>> {
  constexpr static LANGS::LANGUAGE lang = L;
};

template<typename T>
concept Strategy =
requires(
  T& t,
  Base::Types::Source s) {

  { t(s) } -> std::same_as<void>;
  { t() } -> std::same_as<void>;
};

template<LANGS::LANGUAGE lang>
struct StraBase {
  StraBase(Rule<lang>& rule): rule_{rule} {}
protected:
  Rule<lang>& rule_;
};

///////////////////////////////////////////////////////////////////////////////
//                   Implementation of concrete Strategies                   //
///////////////////////////////////////////////////////////////////////////////
template<LANGS::LANGUAGE lang>
class MatchStra: public StraBase<lang> {
public:
  MatchStra(Rule<lang>& rule):
    StraBase<lang>{rule} {
    static_assert(Strategy<MatchStra>);
  }

  void operator()(Base::Types::Source s) {

  }

  void operator()() {
    PM::MatchResult<AbstTree<lang>> result =
      Parser::AST::PatternMatch::Matching(
        this->rule_.runtime_ctx.binded_tree,
        this->rule_.GetSourceTree());
  }
};

template<LANGS::LANGUAGE lang>
class WhereStra: public StraBase<lang> {
public:
  WhereStra(Rule<lang>& rule):
    StraBase<lang>{rule} {
    static_assert(Strategy<WhereStra>);
  }

  void operator()(Base::Types::Source source) {

  }

  void operator()() {

  }
};

template<LANGS::LANGUAGE lang>
class BuildStra: public StraBase<lang> {
public:
  BuildStra(Rule<lang>& rule): StraBase<lang>{rule} {
    static_assert(Strategy<BuildStra>);
  }

  void operator()(Base::Types::Source source) {

  }

  void operator()() {

  }
};

template<LANGS::LANGUAGE lang>
using FirstOrderStra_t = std::variant<
  MatchStra<lang>, WhereStra<lang>, BuildStra<lang>>;
template<LANGS::LANGUAGE lang>
struct FirstOrderStra: public StraBase<lang> {

  FirstOrderStra(std::initializer_list<FirstOrderStra_t<lang>> stras):
    StraBase<lang>{stras.size() > 0 ? static_cast<StraBase<lang>&>(*stras.begin()).rule_ : nullptr } {
    static_assert(Strategy<FirstOrderStra>);

    #if !NDEBUG
    Rule<lang>* rule = nullptr;
    for (auto& s: stras) {
      if (rule == nullptr) {
        rule = static_cast<StraBase<lang>&>(s).rule_;
      } else {
        ASSERT(rule == static_cast<StraBase<lang>&>(s).rule_);
      }
    }
    #endif
  }

  void operator()(Base::Types::Source source) {

  }

  void operator()() {
    std::for_each(
      stras.begin(), stras.end(),
      [](FirstOrderStra_t<lang>& stra) {

      });
  }

  std::vector<FirstOrderStra_t<lang>> stras;
};

} // LANTr::Transformer

#endif /* LANTR_TRANSFORMER_STRATEGY_H_ */
