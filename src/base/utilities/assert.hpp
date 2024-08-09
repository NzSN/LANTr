#ifndef LANTR_BASE_UTILITY_ASSERT_H_
#define LANTR_BASE_UTILITY_ASSERT_H_

#include <format>
#include <iostream>

#ifdef NDEBUG
#define ASSERT(COND, MSG)
#else

#if __APPLE__
#include <TargetConditionals.h>

#if TARGET_OS_MAC

#define ASSERT_FAILURE_MESSAGE(COND)              \
  "Assertion failed with condition"               \
  << #COND                                        \
  << " at "                                       \
  << std::string{__FILE__} + ":" + std::to_string(__LINE__)
#endif

#else

#define ASSERT_FAILURE_MESSAGE(COND) \
  std::format(                                              \
    "Assertion failed with condition ({}) at {}:{} \n",     \
    #COND, __FILE__, std::to_string(__LINE__))

#endif

#define ASSERT_WITH_MSG(...)                 \
  ([&]{                                      \
    if (!(__VA_ARGS__)) {                    \
      std:: cout << ASSERT_FAILURE_MESSAGE(__VA_ARGS__); \
      std::abort();                          \
    }                                        \
  })()

#define GET_MACRO(_1,_2,NAME,...) NAME
#define ASSERTM(MSG, ...)                       \
  ASSERT_WITH_MSG(__VA_ARGS__)
#define ASSERT(...)                             \
  ASSERT_WITH_MSG(__VA_ARGS__)

#ifdef NO_SLOW_ASSERT
#define ASSERT_SLOW(COND, MSG)
#else

#define ASSERT_SLOW_WITH_MSG(MSG, COND) ASSERTM(MSG, COND)
#define ASSERT_SLOW_WITHOUT_MSG(COND) ASSERT(COND)

#define ASSERT_SLOW(...) ASSERT_SLOW_WITHOUT_MSG(__VA_ARGS__)
#define ASSERTM_SLOW(MSG, ...) ASSERT_SLOW_WITH_MSG(MSG, __VA_ARGS__)

#endif // NO_SLOW_ASSERT

#endif // NDEBUG

#endif // LANTR_BASE_UTILITY_ASSERT_H_
