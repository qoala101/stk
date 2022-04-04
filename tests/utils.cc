#include "utils.h"

#include <gtest/gtest.h>

TEST(Utils, GetUnixTimeFromString) {
  const auto date = stonks::utils::GetUnixTimeMillisFromString("04 Apr 2022", "%d %b %Y");
  ASSERT_TRUE(date.has_value());
  EXPECT_EQ(*date, 1649030400000);

  const auto time = stonks::utils::GetUnixTimeMillisFromString("18:35:44");
  ASSERT_FALSE(time.has_value());

  const auto date_time =
      stonks::utils::GetUnixTimeMillisFromString("4 Apr 2022 18:35:44");
  ASSERT_TRUE(date_time.has_value());
  EXPECT_EQ(*date_time, 1649097344000);

  const auto date_time_leading_zero =
      stonks::utils::GetUnixTimeMillisFromString("04 Apr 2022 18:35:44");
  ASSERT_TRUE(date_time_leading_zero.has_value());
  EXPECT_EQ(*date_time_leading_zero, 1649097344000);
}