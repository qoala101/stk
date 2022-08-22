#include "finance_conversions.h"

#include <gtest/gtest.h>

#include <chrono>

TEST(FinanceConversions, CandleFromKline) {
  const auto kline = stonks::binance::Kline{
      .open_time = std::chrono::milliseconds{1647820800000},
      .open_price = 41256.16,
      .high_price = 42293.25,
      .low_price = 21213.92,
      .close_price = 41004.51,
      .volume = 3880.275361,
      .close_time = std::chrono::milliseconds{1647907199999},
      .quote_asset_volume = 159422440.52710729,
      .num_trades = 161896,
      .taker_buy_base_asset_volume = 2357.304304,
      .taker_buy_quote_asset_volume = 96848939.73882025};
  const auto candle = stonks::finance::ParseCandleFromBinanceKline(kline);

  EXPECT_EQ(candle.open_time, kline.open_time);
  EXPECT_EQ(candle.close_time, kline.close_time);
  EXPECT_EQ(candle.data->open_price, kline.open_price);
  EXPECT_EQ(candle.data->high_price, kline.high_price);
  EXPECT_EQ(candle.data->low_price, kline.low_price);
  EXPECT_EQ(candle.data->close_price, kline.close_price);
  EXPECT_EQ(candle.data->volume, kline.volume);
}