#include "finance_api.h"

#include <gtest/gtest.h>

#include <range/v3/all.hpp>

#include "finance_enum_conversions.h"
#include "utils.h"

namespace {
const auto kDefaultSymbol =
    stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
}  // namespace

TEST(FinanceApi, GetCandlesConsecutive) {
  const auto interval = stonks::finance::Interval::k1Minute;
  auto open_time = stonks::utils::FloorTo<std::chrono::hours>(
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  const auto interval_time =
      stonks::finance::ConvertIntervalToMilliseconds(interval);

  auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, interval, open_time, open_time + interval_time);
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time, open_time);
  EXPECT_TRUE(candles->back().data.has_value());

  open_time += interval_time;
  candles = stonks::finance::GetCandles(kDefaultSymbol, interval, open_time,
                                        open_time + interval_time);
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time, open_time);
  EXPECT_TRUE(candles->back().data.has_value());

  open_time += interval_time;
  candles = stonks::finance::GetCandles(kDefaultSymbol, interval, open_time,
                                        open_time + interval_time);
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time, open_time);
  EXPECT_TRUE(candles->back().data.has_value());
}

TEST(FinanceApi, GetCandlesMiddleStartTime) {
  const auto start_time =
      stonks::utils::FloorTo<std::chrono::hours>(stonks::utils::GetUnixTime() -
                                                 std::chrono::hours{3}) -
      std::chrono::minutes{30};
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, stonks::finance::Interval::k1Hour, start_time,
      start_time + std::chrono::hours{2} + std::chrono::minutes{10});

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 1);
  EXPECT_EQ(candles->back().open_time.count(),
            (start_time + std::chrono::minutes{30}).count());
  EXPECT_TRUE(candles->back().data.has_value());
}

TEST(FinanceApi, GetCandlesMoreThan1000) {
  auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, stonks::finance::Interval::k1Minute,
      stonks::utils::FloorTo<std::chrono::days>(stonks::utils::GetUnixTime() -
                                                std::chrono::days{3}),
      stonks::utils::FloorTo<std::chrono::days>(stonks::utils::GetUnixTime() -
                                                std::chrono::days{1}));

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 60 * 24 * 2);

  *candles |= ranges::actions::sort | ranges::actions::unique;
  EXPECT_EQ(candles->size(), 60 * 24 * 2);
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_TRUE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesPast) {
  const auto start_time = stonks::utils::FloorTo<std::chrono::days>(
      stonks::utils::GetUnixTime() - std::chrono::years{50});
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, stonks::finance::Interval::k1Day, start_time,
      start_time + std::chrono::days{100});
  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 100);
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_FALSE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto start_time = stonks::utils::FloorTo<std::chrono::minutes>(
      stonks::utils::GetUnixTime() + std::chrono::minutes{1} +
      std::chrono::seconds{1});
  const auto end_time = start_time + std::chrono::minutes{2};
  const auto candles = stonks::finance::GetCandles(kDefaultSymbol, interval,
                                                   start_time, end_time);

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 2);
  const auto candles_time_difference =
      (*candles)[1].open_time - (*candles)[0].open_time;
  EXPECT_EQ(candles_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_TRUE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesPastAndFuture) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, interval,
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() - std::chrono::minutes{1}),
      stonks::utils::FloorTo<std::chrono::minutes>(
          stonks::utils::GetUnixTime() + std::chrono::minutes{1}));

  ASSERT_TRUE(candles.has_value());
  EXPECT_EQ(candles->size(), 2);
  const auto candles_time_difference =
      (*candles)[1].open_time - (*candles)[0].open_time;
  EXPECT_EQ(candles_time_difference,
            stonks::finance::ConvertIntervalToMilliseconds(interval));
  ranges::for_each(*candles, [](const stonks::finance::Candle &candle) {
    EXPECT_TRUE(candle.data.has_value());
  });
}

TEST(FinanceApi, GetCandlesNoEndTime) {
  const auto interval = stonks::finance::Interval::k1Minute;
  const auto candles = stonks::finance::GetCandles(
      kDefaultSymbol, interval,
      stonks::utils::GetUnixTime() - std::chrono::hours{1});
  ASSERT_TRUE(candles.has_value());
  EXPECT_GE(candles->size(), 59);
  EXPECT_LE(candles->size(), 61);
}

// TEST(FinanceApi, PlaceInvalidOrder) {
//   const auto order_request = stonks::finance::OrderRequest{
//       .uuid = stonks::utils::GenerateUuid(),
//       .time = stonks::utils::GetUnixTime(),
//       .buy_or_sell = stonks::finance::BuyOrSell::kSell,
//       .symbol = kDefaultSymbol,
//       .quantity = 2.1,
//       .price = 1500};

//   const auto error = stonks::finance::PlaceOrder(order_request);
//   ASSERT_TRUE(error.has_value());
//   EXPECT_EQ(error->uuid, order_request.uuid);
//   EXPECT_EQ(error->message,
//             "-1111: Precision is over the maximum defined for this asset.");
// }

// namespace {
// auto last_order_uuid = stonks::utils::GenerateUuid();
// }

// TEST(FinanceApi, PlaceValidOrder) {
//   const auto order_request = stonks::finance::OrderRequest{
//       .uuid = last_order_uuid,
//       .time = stonks::utils::GetUnixTime(),
//       .buy_or_sell = stonks::finance::BuyOrSell::kSell,
//       .symbol = kDefaultSymbol,
//       .quantity = 2,  // TODO test with 2.1 which has big percision
//       .price = 1500};

//   const auto error = stonks::finance::PlaceOrder(order_request);
//   ASSERT_FALSE(error.has_value());
// }

// TEST(FinanceApi, GetOrderInfo) {
//   const auto order_info =
//       stonks::finance::GetOrderInfo(kDefaultSymbol, last_order_uuid);
//   ASSERT_TRUE(order_info.has_value());
//   EXPECT_EQ(order_info->uuid, last_order_uuid);
// }

TEST(FinanceApi, GetAverageSymbolPriceGood) {
  const auto price = stonks::finance::GetAverageSymbolPrice(kDefaultSymbol);
  ASSERT_TRUE(price.has_value());
}

TEST(FinanceApi, GetAverageSymbolPriceBad) {
  const auto price =
      stonks::finance::GetAverageSymbolPrice(stonks::finance::Symbol{});
  ASSERT_FALSE(price.has_value());
}

TEST(FinanceApi, GetAllSymbolsPrices) {
  // const auto prices = stonks::finance::GetAllSymbolsPrices();
  // ASSERT_TRUE(prices.has_value());
  // EXPECT_FALSE(prices->empty());
}

TEST(FinanceApi, GetAllSymbols) {
  const auto symbols = stonks::finance::GetAllSymbols();
  ASSERT_TRUE(symbols.has_value());
  EXPECT_FALSE(symbols->empty());
}