#include "bottom.hpp"
#include "base/utilities/assert.hpp"

namespace LANTr {
namespace Base {
namespace Utility {
namespace Bottom {


void Unreachable(Reason reason) {
  ASSERTM("Not recognized reason",
         reason >= Reason::FIRST_REASON &&
         reason < Reason::END_OF_REASON);

  throw ReachBottom(reason);
}

} // Bottom
} // Utility
} // Base
} // LANTr
