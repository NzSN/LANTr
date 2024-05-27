#ifndef LANTR_API_LANTR_H
#define LANTR_API_LANTR_H

#include "base/types.hpp"

namespace LANTr::Api {

using Source = Base::Types::Source;

enum LANG {
  BEGIN_OF_LANG = 0,
  TypeScript = BEGIN_OF_LANG,
};

struct LANTrConfigs {};

template<LANG lang>
class LANTr {
public:
  Source trans(Source s);
};

} // LANTr::Api

#endif /* LANTR_API_LANTR_H */
