#include "lib.c"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

TEST(test_get_pixel) { ASSERT(get_color(255, 24, 32) == 0xFF1820); }

TEST(test_calc_img_size) {
  ASSERT(calc_img_size(5, 5) == 80);
  ASSERT(calc_img_size(30, 20) == 1840);
}

TEST(test_test_img) {
  struct Bitmap bitmap = test_img(3, 4);
  color_t golden_pixels[] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11};
  ASSERT(memcmp(bitmap.pixels, golden_pixels, 12) == 0);
  free_bitmap(&bitmap);
}

TEST(test_rotate_right_90) {
  struct Bitmap bitmap = test_img(5, 4);
  color_t golden_pixels[] = {15, 10, 5,  0,  16, 11, 6,  1,  17, 12,
                             7,  2,  18, 13, 8,  3,  19, 14, 9,  4};
  rotate_right_90(&bitmap);
  ASSERT(bitmap.width = 4);
  ASSERT(bitmap.height = 5);
  ASSERT(memcmp(bitmap.pixels, golden_pixels, 20) == 0);
  free_bitmap(&bitmap);
}

int main() {
  // Add a `RUN_TEST` line for each test function
  RUN_TEST(test_get_pixel);
  RUN_TEST(test_calc_img_size);
  RUN_TEST(test_test_img);
  RUN_TEST(test_rotate_right_90);
  return failed;
}
