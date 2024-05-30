#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "language_impl.hpp"
#include "parser/languages/language_definitions.hpp"

namespace LANTr::Parser::LANGUAGE {

RC_GTEST_PROP(AntlrParserTest, Parse, ()) {
  LANTr::Base::Types::Source source = "1 + 1";

  {
    ParserImpl parserImpl;
    ParseResult result =
      parserImpl.Parse<ARITH>(source);

    RC_ASSERT(result.tree);
    RC_ASSERT(result.tree->getText() == "1+1");
  }
}

} // LANTr::Parser::LANGUAGE
