#include "candlesticks_stream.h"

#include <gtest/gtest.h>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

TEST(CandlesticksStream, GetOldData) {
  auto stream = stonks::finance::CandlesticksStream{
      stonks::finance::Symbol{"ETH", "USDT"},
      stonks::finance::Interval::k1Minute,
      stonks::utils::GetUnixTime() - std::chrono::hours{2},
      stonks::utils::GetUnixTime() - std::chrono::hours{1}};

  auto candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 60);

  candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_TRUE(candlesticks->empty());
}

TEST(CandlesticksStream, GetOldAndNewData) {
  auto stream = stonks::finance::CandlesticksStream{
      stonks::finance::Symbol{"ETH", "USDT"},
      stonks::finance::Interval::k1Minute,
      stonks::utils::GetUnixTime() - std::chrono::days{1},
      stonks::utils::GetUnixTime() + std::chrono::minutes{1}};

  auto candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1440);

  candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_TRUE(candlesticks->empty());
}

TEST(CandlesticksStream, GetFutureData) {
  auto stream = stonks::finance::CandlesticksStream{
      stonks::finance::Symbol{"ETH", "USDT"},
      stonks::finance::Interval::k1Minute, stonks::utils::GetUnixTime(),
      stonks::utils::GetUnixTime() + std::chrono::minutes{2}};

  auto candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_TRUE(candlesticks->empty());
}

TEST(CandlesticksStream, GetFutureDataNoStop) {
  auto stream = stonks::finance::CandlesticksStream{
      stonks::finance::Symbol{"ETH", "USDT"},
      stonks::finance::Interval::k1Minute, stonks::utils::GetUnixTime()};

  auto candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_EQ(candlesticks->size(), 1);
}