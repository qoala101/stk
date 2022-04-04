#include "finance_enum_conversions.h"

#include <gtest/gtest.h>

TEST(FinanceEnumConversions, ConvertIntervalToMillis) {
  auto millis = stonks::finance::ConvertIntervalToMillis(
      stonks::finance::Interval::k1Minute);
  EXPECT_EQ(millis, 60000);

  millis = stonks::finance::ConvertIntervalToMillis(
      stonks::finance::Interval::k3Minutes);
  EXPECT_EQ(millis, 180000);

  millis = stonks::finance::ConvertIntervalToMillis(
      stonks::finance::Interval::k1Hour);
  EXPECT_EQ(millis, 3600000);

  millis = stonks::finance::ConvertIntervalToMillis(
      stonks::finance::Interval::k1Day);
  EXPECT_EQ(millis, 86400000);
}