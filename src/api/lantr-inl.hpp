#ifndef LANTR_API_LANTR_IMPL_H
#define LANTR_API_LANTR_IMPL_H

#include "lantr.hpp"
#include "base/utility.hpp"

namespace Bottom = LANTr::Base::Utility::Bottom;

namespace LANTr::Api {

template<LANG lang>
Source LANTr<lang>::trans(Source s) {
  Bottom::Unreachable(Bottom::Reason::NOT_IMPLEMENTED);
}

} // LANTr::Api

#endif /* LANTR_API_LANTR_IMPL_H */
