#ifndef LANTR_API_LANTR_H
#define LANTR_API_LANTR_H

#include <vector>

#include "base/types.hpp"
#include "rule.hpp"

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
  LANTr(std::initializer_list<RewriteRule> rules);

  Source trans(Source s);

private:
  std::vector<RewriteRule> rules_;
};

} // LANTr::Api

#endif /* LANTR_API_LANTR_H */
