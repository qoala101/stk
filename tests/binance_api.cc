#include "binance_api.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include <chrono>

TEST(BinanceApi, GetSymbols) {
  const auto symbols = stonks::binance::GetSymbols();
  EXPECT_TRUE(symbols.has_value());
}

TEST(BinanceApi, GetBalances) {
  const auto balances = stonks::binance::GetBalances();
  EXPECT_TRUE(balances.has_value());
}

TEST(BinanceApi, PlaceOrder) {
  const auto place_order_result = stonks::binance::PlaceOrder(
      "BTCUSDT", stonks::binance::OrderSide::kBuy,
      stonks::binance::OrderType::kLimit, 0.01, 20000);
  ASSERT_TRUE(place_order_result.has_value());
  EXPECT_EQ(place_order_result->symbol, "BTCUSDT");
  EXPECT_EQ(place_order_result->side, stonks::binance::OrderSide::kBuy);
  EXPECT_EQ(place_order_result->type, stonks::binance::OrderType::kLimit);
  EXPECT_EQ(place_order_result->price, 20000);
  EXPECT_EQ(place_order_result->time_in_force,
            stonks::binance::OrderTimeInForce::kGoodTillCanceled);
}

TEST(BinanceApi, GetKlines) {
  const auto klines = stonks::binance::GetKlines(
      "BTCUSDT", stonks::binance::CandleInterval::k1Day, std::nullopt,
      std::nullopt, 10);

  ASSERT_TRUE(klines.has_value());
  EXPECT_EQ(klines->size(), 10);

  for (auto iter = std::next(klines->begin()); iter != klines->end(); ++iter) {
    EXPECT_GT(iter->open_time, std::chrono::milliseconds{0});
    EXPECT_LT(iter->open_time, iter->close_time);
    EXPECT_LT(std::prev(iter)->open_time, iter->open_time);
  }
}