#include "candlesticks_stream.h"

#include <gtest/gtest.h>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}

TEST(CandlesticksStream, GetOldData) {
  auto stream = stonks::finance::CandlesticksStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::hours{2}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::hours{1})};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 60);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_TRUE(candlesticks.empty());
}

TEST(CandlesticksStream, GetOldAndNewData) {
  auto stream = stonks::finance::CandlesticksStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::days{1}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{1})};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1440);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_TRUE(candlesticks.empty());
}

TEST(CandlesticksStream, GetFutureData) {
  auto stream = stonks::finance::CandlesticksStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime()),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{2})};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_TRUE(candlesticks.empty());
}

TEST(CandlesticksStream, GetFutureDataNoStop) {
  auto stream = stonks::finance::CandlesticksStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::GetUnixTime()};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);
}