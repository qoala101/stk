#include "finance_api.h"

#include <gtest/gtest.h>

#include <range/v3/all.hpp>

#include "finance_enum_conversions.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}  // namespace

TEST(FinanceApi, GetCandlesticksConsecutive) {
  const auto interval = stonks::finance::Interval::k1Minute;
  auto open_time = stonks::utils::FloorTo<std::chrono::hours>(
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  const auto interval_time =
      stonks::finance::ConvertIntervalToMilliseconds(interval);

  auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, interval, open_time, open_time + interval_time);
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  EXPECT_EQ(candlesticks->back().open_time, open_time);
  EXPECT_TRUE(candlesticks->back().data.has_value());

  open_time += interval_time;
  candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, interval, open_time, open_time + interval_time);
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  EXPECT_EQ(candlesticks->back().open_time, open_time);
  EXPECT_TRUE(candlesticks->back().data.has_value());

  open_time += interval_time;
  candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, interval, open_time, open_time + interval_time);
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  EXPECT_EQ(candlesticks->back().open_time, open_time);
  EXPECT_TRUE(candlesticks->back().data.has_value());
}

TEST(FinanceApi, GetCandlesticksMiddleStartTime) {
  const auto start_time =
      stonks::utils::FloorTo<std::chrono::hours>(stonks::utils::GetUnixTime() -
                                                 std::chrono::hours{3}) -
      std::chrono::minutes{30};
  const auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, stonks::finance::Interval::k1Hour, start_time,
      start_time + std::chrono::hours{2} + std::chrono::minutes{10});

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
  EXPECT_EQ(candlesticks->back().open_time.count(),
            (start_time + std::chrono::minutes{30}).count());
  EXPECT_TRUE(candlesticks->back().data.has_value());
}

TEST(FinanceApi, GetCandlesticksMoreThan1000) {
  auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::days>(stonks::utils::GetUnixTime() -
                                                std::chrono::days{3}),
      stonks::utils::FloorTo<std::chrono::days>(stonks::utils::GetUnixTime() -
                                                std::chrono::days{1}));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);

  *candlesticks |= ranges::actions::sort | ranges::actions::unique;
  EXPECT_EQ(candlesticks->size(), 60 * 24 * 2);
  ranges::for_each(*candlesticks,
                   [](const stonks::finance::Candlestick &candle) {
                     EXPECT_TRUE(candle.data.has_value());
                   });
}

TEST(FinanceApi, GetCandlesticksPast) {
  const auto start_time = stonks::utils::FloorTo<std::chrono::days>(
      stonks::utils::GetUnixTime() - std::chrono::years{50});
  const auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, stonks::finance::Interval::k1Day, start_time,
      start_time + std::chrono::days{100});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 100);
  ranges::for_each(*candlesticks,
                   [](const stonks::finance::Candlestick &candle) {
                     EXPECT_FALSE(candle.data.has_value());
                   });
}

TEST(FinanceApi, GetCandlesticksFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto start_time = stonks::utils::FloorTo<std::chrono::minutes>(
      stonks::utils::GetUnixTime() + std::chrono::minutes{1} +
      std::chrono::seconds{1});
  const auto end_time = start_time + std::chrono::minutes{2};
  const auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, interval, start_time, end_time);

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 2);
  const auto candlesticks_time_difference =
      (*candlesticks)[1].open_time - (*candlesticks)[0].open_time;
  EXPECT_EQ(candlesticks_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
  ranges::for_each(*candlesticks,
                   [](const stonks::finance::Candlestick &candle) {
                     EXPECT_TRUE(candle.data.has_value());
                   });
}

TEST(FinanceApi, GetCandlesticksPastAndFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, interval,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::minutes{1}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{1}));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 2);
  const auto candlesticks_time_difference =
      (*candlesticks)[1].open_time - (*candlesticks)[0].open_time;
  EXPECT_EQ(candlesticks_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
  ranges::for_each(*candlesticks,
                   [](const stonks::finance::Candlestick &candle) {
                     EXPECT_TRUE(candle.data.has_value());
                   });
}

TEST(FinanceApi, GetCandlesticksNoEndTime) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candlesticks = stonks::finance::GetCandlesticks(
      kDefaultSymbol, interval,
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_GE(candlesticks->size(), 59);
  EXPECT_LE(candlesticks->size(), 61);
}