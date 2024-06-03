#ifndef LANTR_BASE_UTILITY_BOTTOM_H_
#define LANTR_BASE_UTILITY_BOTTOM_H_

#include <exception>

namespace LANTr {
namespace Base {
namespace Utility {
namespace Bottom {

//#include "base/utilities/assert.hpp"

enum Reason {
  FIRST_REASON = 0,
  NOT_IMPLEMENTED = FIRST_REASON,
  END_OF_REASON
};

namespace {

struct ReachBottom: public std::exception {
  ReachBottom(Reason reason): reason_(reason) {}
  const char* what() const noexcept {
    switch (reason_) {
    case Reason::NOT_IMPLEMENTED:
      return "Not implemented yet";
    default:
      return "Unknown reason";
    }
  }
private:
  Reason reason_;
};

}

void Unreachable(Reason reason);

} // Bottom
} // Utility
} // Base
} // LANTr

#endif // LANTR_BASE_UTILITY_BOTTOM_H_
