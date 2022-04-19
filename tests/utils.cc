#include "utils.h"

#include <gtest/gtest.h>

TEST(Utils, ParseUnixTimeFromString) {
  const auto date =
      stonks::utils::ParseUnixTimeFromString("04 Apr 2022", "%d %b %Y");
  EXPECT_EQ(date, std::chrono::milliseconds{1649030400000});

  const auto time = stonks::utils::ParseUnixTimeFromString("18:35:44");
  EXPECT_EQ(time, std::chrono::milliseconds::zero());

  const auto date_time =
      stonks::utils::ParseUnixTimeFromString("4 Apr 2022 18:35:44");
  EXPECT_EQ(date_time, std::chrono::milliseconds{1649097344000});

  const auto date_time_leading_zero =
      stonks::utils::ParseUnixTimeFromString("04 Apr 2022 18:35:44");
  EXPECT_EQ(date_time_leading_zero, std::chrono::milliseconds{1649097344000});
}

TEST(Utils, ConvertUnixTimeToString) {
  auto time_string = stonks::utils::ConvertUnixTimeToString(
      std::chrono::milliseconds{1649030400000}, "%d %b %Y");
  EXPECT_EQ(time_string, "04 Apr 2022");

  time_string = stonks::utils::ConvertUnixTimeToString(
      std::chrono::milliseconds{1649097344000});
  EXPECT_EQ(time_string, "04 Apr 2022 18:35:44");

  time_string = stonks::utils::ConvertUnixTimeToString(
      std::chrono::milliseconds{1649097344000}, "%^^");
  EXPECT_EQ(time_string, "%^^");
}