#ifndef LANTR_TRANSFORMER_STRATEGY_H_
#define LANTR_TRANSFORMER_STRATEGY_H_

#include "base/types.hpp"
#include "parser/ast/abst_parsetree.hpp"
#include "parser/ast/pattern_matching/pattern_matching.hpp"
#include "parser/parser.hpp"
#include "rule.hpp"
#include "parser/languages/language_definitions.hpp"
#include <algorithm>
#include <initializer_list>
#include <utility>

namespace P = ::LANTr::Parser;
namespace LANGS = P::LANGUAGE;
namespace AST = P::AST;
namespace PM = P::AST::PatternMatch;

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
concept is_strategy =
requires(
  T& t,
  Base::Types::Source s) {

  { t() } -> std::same_as<void>;
};

template<LANGS::LANGUAGE lang>
struct Strategy {
  Strategy(Rule<lang>& rule): rule_{rule} {}
  void BindProgram(Base::Types::Source s) {
    Parser::ParseResult<lang> result = Parser::Parser<lang>::Parse(s);
    this->rule_.runtime_ctx.Bind(std::move(result));
  }

  virtual void operator()() = 0;

protected:
  Rule<lang>& rule_;
};

///////////////////////////////////////////////////////////////////////////////
//                   Implementation of concrete Strategies                   //
///////////////////////////////////////////////////////////////////////////////
template<LANGS::LANGUAGE lang>
class MatchStra: public Strategy<lang> {
public:
  MatchStra(Rule<lang>& rule):
    Strategy<lang>{rule} {
    static_assert(is_strategy<MatchStra>);
  }

  void operator()() override {
    PM::MatchResult<AbstTree<lang>> result =
      Parser::AST::PatternMatch::Matching(
        this->rule_.runtime_ctx.binded_tree,
        this->rule_.GetSourceTree());
  }
};

template<LANGS::LANGUAGE lang>
class WhereStra: public Strategy<lang> {
public:
  WhereStra(Rule<lang>& rule):
    Strategy<lang>{rule} {
    static_assert(is_strategy<WhereStra>);
  }

  void operator()() override {

  }
};

template<LANGS::LANGUAGE lang>
class BuildStra: public Strategy<lang> {
public:
  BuildStra(Rule<lang>& rule): Strategy<lang>{rule} {
    static_assert(is_strategy<BuildStra>);
  }

  void operator()() override {

  }
};

} // LANTr::Transformer

#endif /* LANTR_TRANSFORMER_STRATEGY_H_ */
