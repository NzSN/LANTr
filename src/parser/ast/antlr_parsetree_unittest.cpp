#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "base/tree_concepts.hpp"
#include "parser/ast/antlr_parsetree.hpp"
#include "parser/languages/language_definitions.hpp"
#include "parser/languages/language_impl.hpp"

namespace LANTr::Parser::AST {

using ParserImpl = LANTr::Parser::LANGUAGE::ParserImpl;
using LANGUAGES = LANTr::Parser::LANGUAGE::LANGUAGE;
using ParserResult = LANTr::Parser::LANGUAGE::ParseResult<LANGUAGES::ARITH>;

struct AstTest: public ::testing::Test {

bool equivalent(LANGUAGE::TreeType<LANGUAGE::ANTLR4>::type* ltree,
                const AntlrTree* rtree) {

  if (rtree->GetInternal() != ltree ||
      rtree->size() != ltree->children.size()) {
    return false;
  }

  Base::TreeConcepts::GetChildren(rtree);
  Base::TreeConcepts::GetParent(const_cast<AntlrTree*>(rtree));

  auto iter = ltree->children.cbegin();
  for (auto const& c: rtree->GetChildren()) {
    ASSERT(iter < ltree->children.cend(), "out of range");

    const InternalTree* internalTree = c->GetInternal();
    if (internalTree != *iter) {
      return false;
    }

    if (!equivalent(*iter, c.get())) {
      return false;
    }

    ++iter;
  }

  return true;
}

};

RC_GTEST_FIXTURE_PROP(AstTest, Construction, ()) {
  LANTr::Base::Types::Source source =
    "1+2+3+(2*2)";

  ParserImpl parser;
  ParserResult result = parser.Parse<LANGUAGES::ARITH>(source);

  std::unique_ptr<AntlrTree> tree = AntlrTree::BuildFrom(result.tree);

  // Assert that the generated AntlrTree is equivalent the tree
  // generated by ParserImpl.
  RC_ASSERT(equivalent(result.tree, tree.get()) &&
            tree->IsLayerEquivalent());
}

} // LANTr::Parser::AST
