#include "bottom.hpp"
#include "base/utilities/assert.hpp"

namespace LANTr {
namespace Base {
namespace Utility {
namespace Bottom {


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
