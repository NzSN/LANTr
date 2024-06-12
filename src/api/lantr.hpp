#ifndef LANTR_API_LANTR_H_
#define LANTR_API_LANTR_H_

#include <vector>

#include "base/types.hpp"
#include "rule.hpp"

#include "base/utilities/bottom.hpp"

namespace LANTr::Api {

namespace {
using Source = Base::Types::Source;
}

enum LANG {
  FIRST_LANG = 0,
  TypeScript = FIRST_LANG,
  END_OF_LANG,
};

template<LANG lang>
class LANTr {
public:
  LANTr(std::initializer_list<RewriteRule> rules):
    rules_{rules} {}

  Source operator()(Source s) {
    Base::Utility::Bottom::Unreachable(
      Base::Utility::Bottom::Reason::NOT_IMPLEMENTED);
  }

private:
  std::vector<RewriteRule> rules_;
};

} // LANTr::Api

#endif // LANTR_API_LANTR_H_
