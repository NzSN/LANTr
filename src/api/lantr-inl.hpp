#ifndef LANTR_API_LANTR_IMPL_H
#define LANTR_API_LANTR_IMPL_H

#include "lantr.hpp"
#include "base/utility.hpp"

namespace LANTr::Api {

namespace {
using Base::Utility::Bottom::Unreachable;
using Base::Utility::Bottom::Reason;
}

template<LANG lang>
Source LANTr<lang>::trans(Source s) {
  Unreachable(Reason::NOT_IMPLEMENTED);
}

} // LANTr::Api

#endif /* LANTR_API_LANTR_IMPL_H */
