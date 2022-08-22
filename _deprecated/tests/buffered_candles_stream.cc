#include "buffered_candles_stream.h"

#include <gtest/gtest.h>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}

TEST(BufferedCandlesStream, GetOldData) {
  auto stream = stonks::finance::BufferedCandlesStream{
      stonks::finance::CandlesStream{
          kDefaultSymbol, stonks::finance::Interval::k1Minute,
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() - std::chrono::hours{2}),
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() - std::chrono::hours{1})},
      25};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 25);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 25);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 10);

  candles = stream.GetNextCandles();
  EXPECT_TRUE(candles.empty());
}

TEST(BufferedCandlesStream, GetOldAndNewData) {
  auto stream = stonks::finance::BufferedCandlesStream{
      stonks::finance::CandlesStream{
          kDefaultSymbol, stonks::finance::Interval::k1Minute,
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() - std::chrono::days{1}),
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() + std::chrono::minutes{1})},
      1000};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1000);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 440);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);

  candles = stream.GetNextCandles();
  EXPECT_TRUE(candles.empty());
}

TEST(BufferedCandlesStream, GetFutureDataNoStop) {
  auto stream = stonks::finance::BufferedCandlesStream{
      stonks::finance::CandlesStream{kDefaultSymbol,
                                     stonks::finance::Interval::k1Minute,
                                     stonks::utils::GetUnixTime()},
      1000};

  auto candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);

  candles = stream.GetNextCandles();
  EXPECT_EQ(candles.size(), 1);
}