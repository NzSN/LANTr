#ifndef LANTR_TRANSFORMER_STRATEGY_H_
#define LANTR_TRANSFORMER_STRATEGY_H_

#include "base/types.hpp"
#include "parser/ast/abst_parsetree.hpp"
#include "parser/ast/pattern_matching/pattern_matching.hpp"
#include "rule.hpp"
#include "parser/languages/language_definitions.hpp"
#include <algorithm>
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

template<typename T>
concept ConcreteStrategy =
requires(T& t,
  Base::Types::Source s,
  AbstTree<LangOfStrategy<T>::lang> tree) {
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
    static_assert(ConcreteStrategy<MatchStra<lang>>);
  }

  void operator()() {
    PM::MatchResult<AbstTree<lang>> result =
      Parser::AST::PatternMatch::Matching(, this->rule_.GetSourceTree());
  }
};

template<LANGS::LANGUAGE lang>
class WhereStra: public StraBase<lang> {
public:
  WhereStra(Rule<lang>& rule):
    StraBase<lang>{rule} {
    static_assert(ConcreteStrategy<MatchStra<lang>>);
  }

  void operator()(Base::Types::Source source) {

  }
};

template<LANGS::LANGUAGE lang>
using ConcreteStra = std::variant<
  MatchStra<lang>,
  WhereStra<lang>>;
template<LANGS::LANGUAGE lang>
void visitConcreteStra(ConcreteStra<lang>& stra) {
  if (auto& s = stra.template get_if<MatchStra<lang>>()) {
    s();
  } else if (auto& s = stra.template get_if<WhereStra<lang>>()) {
    s();
  } else {
    std::unreachable();
  }
}

} // LANTr::Transformer

#endif /* LANTR_TRANSFORMER_STRATEGY_H_ */
