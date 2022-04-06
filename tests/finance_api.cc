#include "finance_api.h"

#include <gtest/gtest.h>

#include <range/v3/all.hpp>

#include "finance_enum_conversions.h"
#include "utils.h"

TEST(FinanceApi, GetCandlesticks) {
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour,
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 00:00:00"),
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 23:59:59"));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 24);
}

TEST(FinanceApi, GetCandlesticksSingle) {
  const auto open_time =
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 00:00:00");
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour, open_time, open_time);

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  EXPECT_EQ(candlesticks->back().open_time, open_time);
}

TEST(FinanceApi, GetConsecutiveCandlesticks) {
  const auto interval = stonks::finance::Interval::k1Hour;
  const auto first_open_time =
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 01:00:00");

  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", interval, first_open_time, first_open_time);
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  const auto first_candlestick = candlesticks->back();
  EXPECT_EQ(first_candlestick.open_time, first_open_time);

  const auto second_open_time =
      first_open_time +
      stonks::finance::ConvertIntervalToMilliseconds(interval);
  candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", interval, second_open_time, second_open_time);
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  const auto second_candlestick = candlesticks->back();
  EXPECT_EQ(second_candlestick.open_time, second_open_time);
}

TEST(FinanceApi, GetCandlesticksMoreThan1000) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute,
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 00:00:00"),
      stonks::utils::ParseUnixTimeFromString("2 Apr 2022 23:59:59"));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);

  *candlesticks |= ranges::actions::sort | ranges::actions::unique;
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);
}

TEST(FinanceApi, GetCandlesticksWrongDates) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Month,
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 00:00:00"),
      stonks::utils::ParseUnixTimeFromString("1 Apr 2122 23:59:59"));
  EXPECT_FALSE(candlesticks.has_value());

  candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute,
      stonks::utils::ParseUnixTimeFromString("1 Apr 3000 00:00:00"),
      stonks::utils::ParseUnixTimeFromString("1 Apr 3000 23:59:59"));
  EXPECT_FALSE(candlesticks.has_value());
}

TEST(FinanceApi, GetCandlesticksBigNumber) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute,
      stonks::utils::ParseUnixTimeFromString("1 Apr 2022 00:00:00"),
      stonks::utils::ParseUnixTimeFromString("1 Apr 3000 23:59:59"));
  EXPECT_FALSE(candlesticks.has_value());
}