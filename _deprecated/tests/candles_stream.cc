#include "candles_stream.h"

#include <gtest/gtest.h>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}

TEST(CandlesStream, GetOldData) {
  auto stream = stonks::finance::CandlesStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::hours{2}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::hours{1})};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 60);

  candles = stream.GetNextCandles();
  EXPECT_TRUE(candles.empty());
}

TEST(CandlesStream, GetOldAndNewData) {
  auto stream = stonks::finance::CandlesStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::days{1}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{1})};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1440);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);

  candles = stream.GetNextCandles();
  EXPECT_TRUE(candles.empty());
}

TEST(CandlesStream, GetFutureData) {
  auto stream = stonks::finance::CandlesStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime()),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{2})};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);

  candles = stream.GetNextCandles();
  EXPECT_TRUE(candles.empty());
}

TEST(CandlesStream, GetFutureDataNoStop) {
  auto stream = stonks::finance::CandlesStream{
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::GetUnixTime()};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);
}