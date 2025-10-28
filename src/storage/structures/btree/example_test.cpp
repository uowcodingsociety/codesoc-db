#include <gtest/gtest.h>

TEST(EXAMPLE_TEST, ADD_NUMBERS) {
  auto a = 5, b = 10;

  auto result = a + b;
  EXPECT_EQ(result, 15);
}
