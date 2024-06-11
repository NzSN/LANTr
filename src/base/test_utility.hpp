#ifndef LANTR_BASE_TEST_UTILITY_H_
#define LANTR_BASE_TEST_UTILITY_H_

#include <functional>
#include <rapidcheck/gtest.h>

namespace LANTr::Base::RC {

int RandomNumUntil(int min, int max, std::function<bool(int)> predicate);

} // LANTr::Base::RC


#endif /* LANTR_BASE_TEST_UTILITY_H_ */
