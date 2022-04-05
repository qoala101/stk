#include "finance_enum_conversions.h"

#include <gtest/gtest.h>

#include <chrono>

TEST(FinanceEnumConversions, ConvertIntervalToMillis) {
  auto millis = stonks::finance::ConvertIntervalToMilliseconds(
      stonks::finance::Interval::k1Minute);
  EXPECT_EQ(millis, std::chrono::milliseconds{60000});

  millis = stonks::finance::ConvertIntervalToMilliseconds(
      stonks::finance::Interval::k3Minutes);
  EXPECT_EQ(millis, std::chrono::milliseconds{180000});

  millis = stonks::finance::ConvertIntervalToMilliseconds(
      stonks::finance::Interval::k1Hour);
  EXPECT_EQ(millis, std::chrono::milliseconds{3600000});

  millis = stonks::finance::ConvertIntervalToMilliseconds(
      stonks::finance::Interval::k1Day);
  EXPECT_EQ(millis, std::chrono::milliseconds{86400000});
}