#include "finance_api.h"

#include <gtest/gtest.h>

#include <range/v3/all.hpp>

#include "finance_enum_conversions.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}  // namespace

TEST(FinanceApi, GetCandlesConsecutive) {
  const auto interval = stonks::finance::Interval::k1Minute;
  auto open_time = stonks::utils::FloorTo<std::chrono::hours>(
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  const auto interval_time =
      stonks::finance::ConvertIntervalToMilliseconds(interval);

  auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, interval, open_time, open_time + interval_time);
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time, open_time);
  EXPECT_TRUE(candles->back().data.has_value());

  open_time += interval_time;
  candles = stonks::finance::GetCandles(kDefaultSymbol, interval, open_time,
                                        open_time + interval_time);
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time, open_time);
  EXPECT_TRUE(candles->back().data.has_value());

  open_time += interval_time;
  candles = stonks::finance::GetCandles(kDefaultSymbol, interval, open_time,
                                        open_time + interval_time);
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time, open_time);
  EXPECT_TRUE(candles->back().data.has_value());
}

TEST(FinanceApi, GetCandlesMiddleStartTime) {
  const auto start_time =
      stonks::utils::FloorTo<std::chrono::hours>(stonks::utils::GetUnixTime() -
                                                 std::chrono::hours{3}) -
      std::chrono::minutes{30};
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, stonks::finance::Interval::k1Hour, start_time,
      start_time + std::chrono::hours{2} + std::chrono::minutes{10});

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time.count(),
            (start_time + std::chrono::minutes{30}).count());
  EXPECT_TRUE(candles->back().data.has_value());
}

TEST(FinanceApi, GetCandlesMoreThan1000) {
  auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::days>(stonks::utils::GetUnixTime() -
                                                std::chrono::days{3}),
      stonks::utils::FloorTo<std::chrono::days>(stonks::utils::GetUnixTime() -
                                                std::chrono::days{1}));

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 60 * 24 * 2);

  *candles |= ranges::actions::sort | ranges::actions::unique;
  EXPECT_EQ(candles->size(), 60 * 24 * 2);
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_TRUE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesPast) {
  const auto start_time = stonks::utils::FloorTo<std::chrono::days>(
      stonks::utils::GetUnixTime() - std::chrono::years{50});
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, stonks::finance::Interval::k1Day, start_time,
      start_time + std::chrono::days{100});
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 100);
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_FALSE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto start_time = stonks::utils::FloorTo<std::chrono::minutes>(
      stonks::utils::GetUnixTime() + std::chrono::minutes{1} +
      std::chrono::seconds{1});
  const auto end_time = start_time + std::chrono::minutes{2};
  const auto candles = stonks::finance::GetCandles(kDefaultSymbol, interval,
                                                   start_time, end_time);

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 2);
  const auto candles_time_difference =
      (*candles)[1].open_time - (*candles)[0].open_time;
  EXPECT_EQ(candles_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_TRUE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesPastAndFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, interval,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::minutes{1}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{1}));

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 2);
  const auto candles_time_difference =
      (*candles)[1].open_time - (*candles)[0].open_time;
  EXPECT_EQ(candles_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_TRUE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesNoEndTime) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, interval,
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  ASSERT_TRUE(candles.has_value());
  EXPECT_GE(candles->size(), 59);
  EXPECT_LE(candles->size(), 61);
}