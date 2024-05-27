#ifndef LANTR_API_LANTR_IMPL_H
#define LANTR_API_LANTR_IMPL_H

#include "lantr.hpp"
#include "base/utility.hpp"

namespace LANTr::Api {

namespace {
namespace Bottom = Base::Utility::Bottom;
}

template<LANG lang>
LANTr<lang>::LANTr(std::initializer_list<RewriteRule> rules):
  rules_{rules} {}

template<LANG lang>
Source LANTr<lang>::trans(Source s) {
  Bottom::Unreachable(Bottom::Reason::NOT_IMPLEMENTED);
}

} // LANTr::Api

#endif /* LANTR_API_LANTR_IMPL_H */
