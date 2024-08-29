#ifndef LANTR_TRANSFORMER_RULE_H_
#define LANTR_TRANSFORMER_RULE_H_

#include "base/utilities/assert.hpp"
#include "base/n_ary_tree.hpp"
#include "base/tree_layer.hpp"
#include "base/types.hpp"
#include "parser/ast/abst_parsetree.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/languages/language_impl.hpp"
#include "parser/parser.hpp"
#include <stdexcept>

namespace Types = ::LANTr::Base::Types;
namespace LANGS = ::LANTr::Parser::LANGUAGE;

namespace LANTr::Transformer {


template<LANGS::LANGUAGE lang>
class Rule {
public:
  using LangImpl = LANGS::ImplOfLang<lang>;
  using TreeType = LANGS::TreeType<LangImpl::impl>;
  using Tree     = typename TreeType::type;

  Rule(Types::Source source_, Types::Source target_):
    source{source_}, target{target_},
    env_{([&] {
      Parser::ParseResult<lang> result =
        Parser::Parser<lang>::Parse(source);
      if (result.HasErrors()) {
        throw std::runtime_error("Failed to parse source pattern");
      }

      return result;
    })()},
    source_tree{env_.tree}

    {
      ASSERT(source_tree != nullptr);
    }

  /* Only when transfrom between the same language */
  [[nodiscard]]
  Parser::LANGUAGE::ParseResult<lang>
  GenTargetAST() const {
    LANGS::ParseResult<lang> result =
      Parser::Parser<lang>::Parse(target);
    if (result.HasErrors()) {
      throw std::runtime_error("Failed to parse source pattern");
    }
    return result;
  }

  [[nodiscard]]
  const Parser::AST::AbstTree<Tree>*
  GetSourceTree() const {
    return source_tree;
  }

  const Types::Source&
  GetSource() const {
    return source;
  }

  const Types::Source
  GetTarget() const {
    return target;
  }

private:
  Types::Source source;
  Types::Source target;

  Parser::ParseResult<lang> env_;

  /* Only Tree of source pattern is generated due to
   * target may use as template to generate lang that
   * different from source lang. */
  Parser::AST::AbstTree<Tree>* source_tree;
};

} // LANTr::Trans::Strategy

#endif /* LANTR_TRANSFORMER_RULE_H_ */
