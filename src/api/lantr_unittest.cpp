#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "base/types.hpp"
#include "lantr.hpp"

namespace LANTr::Api {

RC_GTEST_PROP(LANTrApp, Main, ()) {
  LANTrConfigs configs{};
  LANTr<LANG::TypeScript> tr{
    {
      // Rewrite Rules
      {"Hello", "World"},
      {"World", "Hello"}
    },
    configs
  };

  Base::Types::Source source{
    "let message: string = 'Hello, World!';"
    "console.log(message);"
  };

  Base::Types::Source transed{tr.trans(source)};
  Base::Types::Source expected{
    "let message: string = 'World, Hello!';"
    "console.log(message);"
  };

  RC_ASSERT(transed == expected);
}

} // LANTr::Api
