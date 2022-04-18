#include "json_conversions.h"

#include <cpprest/json.h>
#include <gtest/gtest.h>

#include <chrono>

TEST(JsonConversions, PlaceOrderResult) {
  const auto raw_json = R"({
  "clientOrderId":"0PaNMDgehJmHYC9UApeK2y",
  "cummulativeQuoteQty":"123456789.123456789",
  "executedQty":"123456789.123456789",
  "fills":[],
  "orderId":123456789,
  "orderListId":-123456789,
  "origQty":"123456789.123456789",
  "price":"123456789.123456789",
  "side":"BUY",
  "status":"NEW",
  "symbol":"BTCUSDT",
  "timeInForce":"GTC",
  "transactTime":1648412181820,
  "type":"LIMIT"
})";
  const auto object = stonks::binance::PlaceOrderResult{
      .client_order_id = "0PaNMDgehJmHYC9UApeK2y",
      .cummulative_quote_quantity = 123456789.123456789,
      .executed_quantity = 123456789.123456789,
      // .fills
      .order_id = 123456789,
      .order_list_id = -123456789,
      .original_quantity = 123456789.123456789,
      .price = 123456789.123456789,
      .side = stonks::binance::Side::kBuy,
      .status = stonks::binance::Status::kNew,
      .symbol = "BTCUSDT",
      .time_in_force = stonks::binance::TimeInForce::kGoodTillCanceled,
      .transaction_time = std::chrono::milliseconds{1648412181820},
      .type = stonks::binance::Type::kLimit};

  const auto parsed_json = web::json::value::parse(raw_json);
  const auto parsed_object =
      stonks::ParseFromJson<stonks::binance::PlaceOrderResult>(parsed_json);

  ASSERT_TRUE(parsed_object.has_value());
  EXPECT_EQ(*parsed_object, object);
}

TEST(JsonConversions, Kline) {
  const auto raw_json = R"([
  1647820800000,
  "41256.16000000",
  "42293.25000000",
  "21213.92000000",
  "41004.51000000",
  "3880.27536100",
  1647907199999,
  "159422440.52710729",
  161896,
  "2357.30430400",
  "96848939.73882025",
  "0"
])";
  const auto object = stonks::binance::Kline{
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

  const auto parsed_json = web::json::value::parse(raw_json);
  const auto parsed_object =
      stonks::ParseFromJson<stonks::binance::Kline>(parsed_json);

  ASSERT_TRUE(parsed_object.has_value());
  EXPECT_EQ(*parsed_object, object);
}

TEST(JsonConversions, Klines) {
  const auto raw_json = R"([
  [
    1647820800000,
    "41256.16000000",
    "42293.25000000",
    "21213.92000000",
    "41004.51000000",
    "3880.27536100",
    1647907199999,
    "159422440.52710729",
    161896,
    "2357.30430400",
    "96848939.73882025",
    "0"
  ],
  [
    1647907200000,
    "41003.64000000",
    "60000.00000000",
    "40419.49000000",
    "42364.14000000",
    "3985.02355200",
    1647993599999,
    "169122058.79004571",
    172787,
    "2437.26573300",
    "103435583.49075382",
    "0"
  ]
])";
  const auto object = std::vector<stonks::binance::Kline>{
      stonks::binance::Kline{
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
          .taker_buy_quote_asset_volume = 96848939.73882025},
      stonks::binance::Kline{
          .open_time = std::chrono::milliseconds{1647907200000},
          .open_price = 41003.64,
          .high_price = 60000,
          .low_price = 40419.49,
          .close_price = 42364.14,
          .volume = 3985.023552,
          .close_time = std::chrono::milliseconds{1647993599999},
          .quote_asset_volume = 169122058.79004571,
          .num_trades = 172787,
          .taker_buy_base_asset_volume = 2437.265733,
          .taker_buy_quote_asset_volume = 103435583.49075382}};

  const auto parsed_json = web::json::value::parse(raw_json);
  const auto parsed_object =
      stonks::ParseFromJson<std::vector<stonks::binance::Kline>>(parsed_json);

  ASSERT_TRUE(parsed_object.has_value());
  EXPECT_EQ(*parsed_object, object);
}

TEST(JsonConversions, BrokenKlineInArray) {
  const auto raw_json = R"([
  [
    1647820800000,
    "41256.16000000",
    "42293.25000000",
    "21213.92000000",
    "41004.51000000",
    "3880.27536100",
    1647907199999,
    "159422440.52710729",
    161896,
    "2357.30430400",
    "96848939.73882025",
    "0"
  ],
  [
    "should be number",
    "41003.64000000",
    "60000.00000000",
    "40419.49000000",
    "42364.14000000",
    "3985.02355200",
    1647993599999,
    "169122058.79004571",
    172787,
    "2437.26573300",
    "103435583.49075382",
    "0"
  ]
])";

  const auto parsed_json = web::json::value::parse(raw_json);
  const auto parsed_object =
      stonks::ParseFromJson<std::vector<stonks::binance::Kline>>(parsed_json);

  ASSERT_FALSE(parsed_object.has_value());
}

TEST(JsonConversions, StrategyOrderRequest) {
  const auto original_object = stonks::finance::StrategyOrderRequest{
      .strategy_info = stonks::finance::StrategyInfo{.name = "breakout"},
      .order_request = stonks::finance::OrderRequest{
          .time = std::chrono::milliseconds{1647820800000},
          .buy_or_sell = stonks::finance::BuyOrSell::kBuy,
          .symbol = stonks::finance::Symbol{.base_asset = "ETH",
                                            .quote_asset = "USDT"},
          .quantity = 123456789.123456789,
          .price = 123456789.123456789,
      }};
  const auto json = stonks::ConvertToJson(original_object);
  const auto parsed_object =
      stonks::ParseFromJson<stonks::finance::StrategyOrderRequest>(json);

  ASSERT_TRUE(parsed_object.has_value());
  EXPECT_EQ(original_object, *parsed_object);
}

TEST(JsonConversions, NonParsableObjectProperty) {
  const auto raw_json = R"({
  "buy_or_sell":"BUY",
  "price":123456789.12345679,
  "quantity":123456789.12345679,
  "symbol":{
    "ERROR":"ETH"
  },
  "time":1647820800000
})";
  const auto parsed_json = web::json::value::parse(raw_json);
  const auto parsed_object =
      stonks::ParseFromJson<stonks::finance::OrderRequest>(parsed_json);

  ASSERT_FALSE(parsed_object.has_value());
}