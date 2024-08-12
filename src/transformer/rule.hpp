#ifndef LANTR_TRANSFORMER_RULE_H_
#define LANTR_TRANSFORMER_RULE_H_

#include "base/n_ary_tree.hpp"
#include "base/tree_layer.hpp"
#include "base/types.hpp"
#include "parser/ast/antlr_parsetree.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/parser.hpp"

namespace Types = ::LANTr::Base::Types;
namespace LANGS = ::LANTr::Parser::LANGUAGE;
namespace Parser = ::LANTr::Parser;

namespace LANTr::Transformer {


template<LANGS::LANGUAGE lang>
struct Rule {
  using LangImpl = LANGS::ImplOfLang<lang>;
  using TreeType = LANGS::TreeType<LangImpl::impl>;
  using Tree = Base::Tree<typename TreeType::type>;
  using AbstTree = Parser::AST::AbstTree<Tree>;

  Rule(Types::Source source_, Types::Source target_):
    source{source_}, target{target_},
    env_{([&] {
      return Parser::Parser<lang>::Parse(source);
    })()},
    source_tree{Base::TreeLayer<AbstTree, Tree>::BuildFrom(env_.tree)}
    {}

  Types::Source source;
  Types::Source target;

  /* Only Tree of source pattern is generated due to
   * target may use as template to generate lang that
   * different from source lang. */
  std::unique_ptr<AbstTree> source_tree;

private:
  Parser::ParseResult<lang> env_;
};

} // LANTr::Trans::Strategy

#endif /* LANTR_TRANSFORMER_RULE_H_ */
