#ifndef LANTR_TRANSFORMER_STRATEGY_H_
#define LANTR_TRANSFORMER_STRATEGY_H_

#include "base/types.hpp"
#include "parser/ast/abst_parsetree.hpp"
#include "rule.hpp"
#include "parser/languages/language_definitions.hpp"

namespace LANGS = ::LANTr::Parser::LANGUAGE;
namespace AST = ::LANTr::Parser::AST;

namespace LANTr::Transformer {

template<LANGS::LANGUAGE lang>
class Strategy {
public:
  using LangImpl = LANGS::ImplOfLang<lang>;
  using TreeType = LANGS::TreeType<LangImpl::impl>;
  using Tree     = Base::Tree<typename TreeType::type>;

  Strategy(Rule<lang>& rule): rule_{rule} {}

  virtual void operator()(Base::Types::Source source) = 0;
  virtual void operator()(AST::AbstTree<Tree>& tree) = 0;
private:
  Rule<lang>& rule_;
};

///////////////////////////////////////////////////////////////////////////////
//                   Implementation of concrete Strategies                   //
///////////////////////////////////////////////////////////////////////////////
template<LANGS::LANGUAGE lang>
class MatchStrategy: public Strategy<lang> {
public:
  MatchStrategy(Rule<lang>& rule): Strategy<lang>{rule} {}

  void operator()(Base::Types::Source source) {

  }

  void operator()(AST::AbstTree<typename Strategy<lang>::Tree>& tree) {

  }
};

} // LANTr::Transformer

#endif /* LANTR_TRANSFORMER_STRATEGY_H_ */
