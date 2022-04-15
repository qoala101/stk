#include "buffered_candlesticks_stream.h"

#include <gtest/gtest.h>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}

TEST(BufferedCandlesticksStream, GetOldData) {
  auto stream = stonks::finance::BufferedCandlesticksStream{
      stonks::finance::CandlesticksStream{
          kDefaultSymbol, stonks::finance::Interval::k1Minute,
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() - std::chrono::hours{2}),
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() - std::chrono::hours{1})},
      25};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 25);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 25);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 10);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_TRUE(candlesticks.empty());
}

TEST(BufferedCandlesticksStream, GetOldAndNewData) {
  auto stream = stonks::finance::BufferedCandlesticksStream{
      stonks::finance::CandlesticksStream{
          kDefaultSymbol, stonks::finance::Interval::k1Minute,
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() - std::chrono::days{1}),
          stonks::utils::FloorTo<std::chrono::minutes>(
              stonks::utils::GetUnixTime() + std::chrono::minutes{1})},
      1000};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1000);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 440);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_TRUE(candlesticks.empty());
}

TEST(BufferedCandlesticksStream, GetFutureDataNoStop) {
  auto stream = stonks::finance::BufferedCandlesticksStream{
      stonks::finance::CandlesticksStream{kDefaultSymbol,
                                          stonks::finance::Interval::k1Minute,
                                          stonks::utils::GetUnixTime()},
      1000};

  auto candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);

  candlesticks = stream.GetNextCandlesticks();
  EXPECT_EQ(candlesticks.size(), 1);
}