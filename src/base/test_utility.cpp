#include <functional>
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace LANTr::Base::RC {

[[nodiscard]]
int RandomNumUntil(int min, int max, std::function<bool(int)> predicate) {
  int num;
  do {
    num = *rc::gen::inRange(min, max);
  } while (!predicate(num));

  return num;
}

} // LANTr::Base
