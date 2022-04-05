#include "finance_api.h"

#include <gtest/gtest.h>

#include <range/v3/all.hpp>

#include "utils.h"

TEST(FinanceApi, GetCandlesticks) {
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour,
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 00:00:00"),
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 23:59:59"));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 24);
}

TEST(FinanceApi, GetCandlesticksSingle) {
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour,
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 00:00:00"),
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 00:00:00"));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
}

TEST(FinanceApi, GetCandlesticksMoreThan1000) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute,
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 00:00:00"),
      *stonks::utils::GetUnixTimeFromString("2 Apr 2022 23:59:59"));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);

  *candlesticks |= ranges::actions::sort | ranges::actions::unique;
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);
}

TEST(FinanceApi, GetCandlesticksWrongDates) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Month,
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 00:00:00"),
      *stonks::utils::GetUnixTimeFromString("1 Apr 2122 23:59:59"));
  EXPECT_FALSE(candlesticks.has_value());

  candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute,
      *stonks::utils::GetUnixTimeFromString("1 Apr 3000 00:00:00"),
      *stonks::utils::GetUnixTimeFromString("1 Apr 3000 23:59:59"));
  EXPECT_FALSE(candlesticks.has_value());
}

TEST(FinanceApi, GetCandlesticksBigNumber) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute,
      *stonks::utils::GetUnixTimeFromString("1 Apr 2022 00:00:00"),
      *stonks::utils::GetUnixTimeFromString("1 Apr 3000 23:59:59"));
  EXPECT_FALSE(candlesticks.has_value());
}