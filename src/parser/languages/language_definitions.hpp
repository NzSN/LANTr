#ifndef LANTR_PARSER_LANGUAGES_LANGUAGE_DEFINITIONS_H_
#define LANTR_PARSER_LANGUAGES_LANGUAGE_DEFINITIONS_H_

#include "antlr4-runtime.h"

#include "parser/languages/typescript/TypeScriptLexer.h"
#include "parser/languages/typescript/TypeScriptParser.h"

#include "parser/languages/javascript/JavaScriptLexer.h"
#include "parser/languages/javascript/JavaScriptParser.h"

#include "parser/languages/InternalLang/Arith/ArithLexer.h"
#include "parser/languages/InternalLang/Arith/ArithParser.h"

namespace LANTr::Parser::LANGUAGE {

#define LANG_LIST(V) \
  V(JAVASCRIPT, ANTLR4, antlr4::tree::ParseTree, JavaScriptLexer, JavaScriptParser)    \
  V(TYPESCRIPT, ANTLR4, antlr4::tree::ParseTree, TypeScriptLexer, TypeScriptParser)    \
  V(ARITH, ANTLR4, antlr4::tree::ParseTree, ArithLexer, ArithParser)

#define LANG_ANTLR4_ENTRY_LIST(V)                     \
  V(JAVASCRIPT, program, JavaScriptParser)            \
  V(TYPESCRIPT, program, TypeScriptParser)            \
  V(ARITH, prog, ArithParser)

enum LANGUAGE {
  FIRST_LANG = 0,
  JAVASCRIPT = FIRST_LANG,
  TYPESCRIPT,
  ARITH,
  END_OF_LANG
};

enum LANG_IMPL {
  FIRST_LANG_IMPL = 0,
  ANTLR4 = FIRST_LANG_IMPL,
  END_OF_LANG_IMPL
};

// Language informations
template<LANG_IMPL LI>
struct TreeType;

template<>
struct TreeType<ANTLR4> {
  using type = antlr4::tree::ParseTree;
};
template<LANG_IMPL LI>
using TreeType_t = TreeType<LI>::type;

template<LANGUAGE L>
struct LangLexer;

template<LANGUAGE L>
struct LangParser;

#define LANG_INFORMATION_DEFINE(L, IMPL, TREE, LEXER, PARSER) \
  template<>                                                  \
  struct LangLexer<L> {                                       \
    using type = LEXER;                                       \
  };                                                          \
  template<>                                                  \
  struct LangParser<L> {                                      \
    using type = PARSER;                                      \
  };
LANG_LIST(LANG_INFORMATION_DEFINE)
#undef LANG_INFORMATION_DEFINE

template<LANGUAGE L>
using LangLexer_t = LangLexer<L>::type;
template<LANGUAGE L>
using LangParser_t = LangParser<L>::type;

// Impl mapping
template<LANGUAGE L>
struct ImplOfLang;

#define IMPL_MAPPING(L, IMPL, TREE, LEXER, PARSER) \
  template<>                                       \
  struct ImplOfLang<L> {                           \
    static const LANG_IMPL impl = IMPL;            \
  };
LANG_LIST(IMPL_MAPPING)
#undef IMPL_MAPPING

template<LANGUAGE L>
inline constexpr LANG_IMPL ImplOfLang_v = ImplOfLang<L>::impl;

// Antlr4 Entry
template<LANGUAGE L>
struct Antlr4Entry;
#define ANTLR4_ENTRY_MAPPING(L, ENTRY, PARSER)                          \
  template<>                                                            \
  struct Antlr4Entry<L> {                                         \
    static antlr4::tree::ParseTree* GetEntry(PARSER& parser) {      \
      return parser.ENTRY();                                                              \
    }                                                               \
  };
LANG_ANTLR4_ENTRY_LIST(ANTLR4_ENTRY_MAPPING)
#undef ANTLR4_ENTRY_MAPPING


template<LANGUAGE L, LANG_IMPL I>
concept CanParsedBy = ImplOfLang<L>::impl == I;

} // LANTr::Parser::LANGUAGE

#endif // LANTR_PARSER_LANGUAGES_LANGUAGE_DEFINITIONS_H_
