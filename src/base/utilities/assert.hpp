#ifndef LANTR_BASE_UTILITY_ASSERT_H_
#define LANTR_BASE_UTILITY_ASSERT_H_

#include <iostream>

#ifdef NDEBUG
#define ASSERT(COND, MSG)
#else

#define ASSERT(COND, MSG)                               \
  do {                                                  \
    if (!(COND)) {                                      \
      std::cerr << "Failed to assert: "                 \
                << MSG                                  \
                << std::string(" at ") + __FILE__ + ":" \
                   + std::to_string(__LINE__)           \
                << std::endl;                           \
      std::abort();                                     \
    }                                                   \
  } while (0);

#ifdef NO_SLOW_ASSERT
#define ASSERT_SLOW(COND, MSG)
#else
#define ASSERT_SLOW(COND, MSG) ASSERT(COND, MSG)
#endif // NO_SLOW_ASSERT

#endif // NDEBUG

#endif // LANTR_BASE_UTILITY_ASSERT_H_
