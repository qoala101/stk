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
  const auto place_order_acknowledgement = stonks::binance::PlaceOrder(
      "ETHUSDT", stonks::binance::OrderSide::kBuy,
      stonks::binance::OrderType::kLimit,
      stonks::binance::OrderTimeInForce::kGoodTillCanceled, 0.01, std::nullopt,
      1500);
  ASSERT_TRUE(place_order_acknowledgement.has_value());
  EXPECT_EQ(place_order_acknowledgement->symbol, "ETHUSDT");
}

TEST(BinanceApi, GetAllOrders) {
  const auto orders = stonks::binance::GetAllOrders("ETHUSDT");
  ASSERT_TRUE(orders.has_value());
  ASSERT_TRUE(!orders->empty());
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