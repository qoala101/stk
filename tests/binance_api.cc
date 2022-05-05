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

TEST(BinanceApi, PlaceOrderOk) {
  const auto result = stonks::binance::PlaceOrder(
      "ETHUSDT", stonks::binance::OrderSide::kBuy,
      stonks::binance::OrderType::kLimit,
      stonks::binance::OrderTimeInForce::kGoodTillCanceled, 0.01, std::nullopt,
      1500);
  ASSERT_TRUE(
      std::holds_alternative<stonks::binance::PlaceOrderAcknowledgement>(
          result));
  EXPECT_EQ(std::get<stonks::binance::PlaceOrderAcknowledgement>(result).symbol,
            "ETHUSDT");
}

TEST(BinanceApi, PlaceOrderError) {
  const auto result = stonks::binance::PlaceOrder(
      "ERROR", stonks::binance::OrderSide::kBuy,
      stonks::binance::OrderType::kLimit,
      stonks::binance::OrderTimeInForce::kGoodTillCanceled, 0.1, std::nullopt,
      0.1);
  ASSERT_TRUE(std::holds_alternative<stonks::binance::ApiError>(result));
  EXPECT_EQ(std::get<stonks::binance::ApiError>(result).code, -1121);
  EXPECT_EQ(std::get<stonks::binance::ApiError>(result).message,
            "Invalid symbol.");
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
    EXPECT_GT(iter->open_time, std::chrono::milliseconds::zero());
    EXPECT_LT(iter->open_time, iter->close_time);
    EXPECT_LT(std::prev(iter)->open_time, iter->open_time);
  }
}

TEST(BinanceApi, QueryOrder) {
  const auto order_info = stonks::binance::QueryOrder("ETHUSDT", std::nullopt,
                                                      "bl0L7ilSoT7eESDH1dcJr9");
  ASSERT_TRUE(order_info.has_value());
  EXPECT_EQ(order_info->symbol, "ETHUSDT");
  EXPECT_EQ(order_info->client_order_id, "bl0L7ilSoT7eESDH1dcJr9");
}

TEST(BinanceApi, GetAverageSymbolPriceGood) {
  const auto price = stonks::binance::GetAverageSymbolPrice("ETHUSDT");
  ASSERT_TRUE(price.has_value());
}

TEST(BinanceApi, GetAverageSymbolPriceBad) {
  const auto price = stonks::binance::GetAverageSymbolPrice("WRONG_SYMBOL");
  ASSERT_FALSE(price.has_value());
}

TEST(BinanceApi, GetAllSymbolsPrices) {
  const auto prices = stonks::binance::GetAllSymbolsPrices();
  ASSERT_TRUE(prices.has_value());
  EXPECT_FALSE(prices->empty());
}