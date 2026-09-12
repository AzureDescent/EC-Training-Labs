#ifndef DAY0_TESTS_TEST_UTIL_H
#define DAY0_TESTS_TEST_UTIL_H

#include <cmath>
#include <cstdio>
#include <cstring>

// 极简断言：刻意不引 gtest / catch2，保证一条 g++/cmake 命令就能跑起来
namespace testutil {

inline int& checks() {
  static int n = 0;
  return n;
}

inline int& failures() {
  static int n = 0;
  return n;
}

inline void check_near(const char* label, float actual, float expected, float tol) {
  ++checks();
  if (std::fabs(actual - expected) > tol) {
    ++failures();
    std::printf("  [FAIL] %-28s got %8.2f  want %8.2f\n", label, actual, expected);
  } else {
    std::printf("  [ ok ] %-28s got %8.2f\n", label, actual);
  }
}

// actual / expected 都是能打印成字符串的枚举
inline void check_eq(const char* label, const char* actual, const char* expected) {
  ++checks();
  if (std::strcmp(actual, expected) != 0) {
    ++failures();
    std::printf("  [FAIL] %-28s got %s  want %s\n", label, actual, expected);
  } else {
    std::printf("  [ ok ] %-28s got %s\n", label, actual);
  }
}

inline int report(const char* suite) {
  std::printf("%s: %d/%d passed\n", suite, checks() - failures(), checks());
  return failures() == 0 ? 0 : 1;
}

}  // namespace testutil

#endif  // DAY0_TESTS_TEST_UTIL_H
