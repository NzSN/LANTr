#ifndef LANTR_TRANSFORMER_STRATEGY_H_
#define LANTR_TRANSFORMER_STRATEGY_H_

#include "base/types.hpp"
#include "parser/ast/abst_parsetree.hpp"
#include "parser/ast/pattern_matching/pattern_matching.hpp"
#include "rule.hpp"
#include "parser/languages/language_definitions.hpp"

namespace P = ::LANTr::Parser;
namespace LANGS = P::LANGUAGE;
namespace AST = P::AST;
namespace PM = P::AST::PatternMatch;

namespace LANTr::Transformer {

template<LANGS::LANGUAGE lang>
class Strategy {
public:
  using LangImpl = LANGS::ImplOfLang<lang>;
  using TreeType = LANGS::TreeType<LangImpl::impl>;
  using Tree     = Base::Tree<typename TreeType::type>;
  using AbstTree = AST::AbstTree<Tree>;

  Strategy(Rule<lang>& rule): rule_{rule} {}

  virtual void operator()(Base::Types::Source source) = 0;
  virtual void operator()(AbstTree& tree) = 0;
protected:
  Rule<lang>& rule_;
};

///////////////////////////////////////////////////////////////////////////////
//                   Implementation of concrete Strategies                   //
///////////////////////////////////////////////////////////////////////////////
template<LANGS::LANGUAGE lang>
class MatchStrategy: public Strategy<lang> {
public:
  using AbstTree = typename Strategy<lang>::AbstTree;

  MatchStrategy(Rule<lang>& rule): Strategy<lang>{rule} {}

  void operator()(Base::Types::Source source) override {
    auto parseInfo = P::Parser<lang>::Parse(source);
    this->operator()(parseInfo.tree);
  }

  void operator()(AbstTree& tree) override {
    PM::MatchResult<AbstTree> result =
      Parser::AST::PatternMatch::Matching(tree, this->rule_.GetSourceTree());

  }
};

} // LANTr::Transformer

#endif /* LANTR_TRANSFORMER_STRATEGY_H_ */
