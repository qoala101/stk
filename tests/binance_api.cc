#include "binance_api.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "json_conversions.h"

TEST(BinanceApi, Responses) {
  const auto symbols = stonks::binance::GetSymbols();
  EXPECT_TRUE(symbols.has_value());

  const auto balances = stonks::binance::GetBalances();
  EXPECT_TRUE(balances.has_value());

  const auto place_order_result =
      stonks::binance::PlaceOrder("BTCUSDT", stonks::binance::Side::kBuy,
                                  stonks::binance::Type::kLimit, 0.01, 20000);
  ASSERT_TRUE(place_order_result.has_value());
  EXPECT_EQ(place_order_result->symbol, "BTCUSDT");
  EXPECT_EQ(place_order_result->side, stonks::binance::Side::kBuy);
  EXPECT_EQ(place_order_result->type, stonks::binance::Type::kLimit);
  EXPECT_EQ(place_order_result->price, 20000);
  EXPECT_EQ(place_order_result->time_in_force,
            stonks::binance::TimeInForce::kGoodTillCanceled);
}