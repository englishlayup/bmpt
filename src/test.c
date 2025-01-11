#include "lib.c"
#include <assert.h>
#include <stdio.h>

// Very small test helpers
int failed = 0;
#define TEST(name) void name()
#define RUN_TEST(name)                                                         \
  printf("\n\033[1m%s\n\033[0m", #name);                                       \
  name()
#define ASSERT(expr)                                                           \
  if (!(expr)) {                                                               \
    failed = 1;                                                                \
    printf("\033[0;31mFAIL: %s\n\033[0m", #expr);                              \
  } else {                                                                     \
    printf("\033[0;32mPASS: %s\n\033[0m", #expr);                              \
  }
#define ASSERT_STR_EQ(str1, str2)                                              \
  if (!(strcmp(str1, str2) == 0)) {                                            \
    failed = 1;                                                                \
    printf("\033[0;31mFAIL: %s != %s\n\033[0m", str1, str2);                   \
  } else {                                                                     \
    printf("\033[0;32mPASS: %s == %s\n\033[0m", str1, str2);                   \
  }
// End of test helpers

TEST(test_get_pixel) { ASSERT(get_pixel(255, 24, 32) == 0xFF1820); }

int main() {
  // Add a `RUN_TEST` line for each test function
  RUN_TEST(test_get_pixel);
  return failed;
}
