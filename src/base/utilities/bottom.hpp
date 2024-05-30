#ifndef LANTR_BASE_UTILITY_BOTTOM_H_
#define LANTR_BASE_UTILITY_BOTTOM_H_

#include <utility>
#include <exception>

namespace LANTr {
namespace Base {
namespace Utility {
namespace Bottom {

#include "assert.hpp"

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
    }
  }
private:
  Reason reason_;
};

}

void Unreachable(Reason reason) {
  ASSERT(reason >= Reason::FIRST_REASON &&
         reason < Reason::END_OF_REASON,
         "Not recognized reason");

  throw ReachBottom(reason);
}

} // Bottom
} // Utility
} // Base
} // LANTr

#endif // LANTR_BASE_UTILITY_BOTTOM_H_
