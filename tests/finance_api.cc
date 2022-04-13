#include "finance_api.h"

#include <gtest/gtest.h>

#include <range/v3/all.hpp>

#include "finance_enum_conversions.h"
#include "utils.h"

TEST(FinanceApi, GetCandlesticks) {
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour,
      stonks::utils::GetUnixTime() - std::chrono::days{2},
      stonks::utils::GetUnixTime() - std::chrono::days{1} -
          std::chrono::seconds{1});

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 24);
}

namespace {
std::chrono::milliseconds RoundToMinutes(std::chrono::milliseconds time) {
  return std::chrono::milliseconds{(time.count() / 1000000) * 1000000};
}
}  // namespace

TEST(FinanceApi, GetCandlesticksSingle) {
  const auto open_time =
      RoundToMinutes(stonks::utils::GetUnixTime() - std::chrono::hours{1});
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Minute, open_time, open_time);

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  EXPECT_EQ(candlesticks->back().open_time, open_time);
}

TEST(FinanceApi, GetConsecutiveCandlesticks) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto first_open_time =
      RoundToMinutes(stonks::utils::GetUnixTime() - std::chrono::hours{1});

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
      stonks::utils::GetUnixTime() - std::chrono::days{3},
      stonks::utils::GetUnixTime() - std::chrono::days{1} -
          std::chrono::seconds{1});

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);

  *candlesticks |= ranges::actions::sort | ranges::actions::unique;
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);
}

TEST(FinanceApi, GetCandlesticksPast) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Month,
      stonks::utils::GetUnixTime() - std::chrono::years{50},
      stonks::utils::GetUnixTime() - std::chrono::years{49});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_TRUE(candlesticks->empty());
}

TEST(FinanceApi, GetCandlesticksFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", interval,
      stonks::utils::GetUnixTime() + std::chrono::minutes{1},
      stonks::utils::GetUnixTime() + std::chrono::minutes{3});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 2);
  const auto candlesticks_time_difference =
      (*candlesticks)[1].open_time - (*candlesticks)[0].open_time;
  EXPECT_EQ(candlesticks_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
}

TEST(FinanceApi, GetCandlesticksNoEndTime) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", interval,
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 60);
}

TEST(FinanceApi, GetCandlesticksFutureNoEndTime) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", interval,
      stonks::utils::GetUnixTime() + std::chrono::hours{1});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_TRUE(candlesticks->empty());
}

TEST(FinanceApi, GetCandlesticksPastAndFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", interval,
      stonks::utils::GetUnixTime() - std::chrono::minutes{1},
      stonks::utils::GetUnixTime() + std::chrono::minutes{1});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 2);
  const auto candlesticks_time_difference =
      (*candlesticks)[1].open_time - (*candlesticks)[0].open_time;
  EXPECT_EQ(candlesticks_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
}