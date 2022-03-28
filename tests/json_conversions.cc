#define _TURN_OFF_PLATFORM_STRING

#include "json_conversions.h"

#include <cpprest/json.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(JsonConversions, PlaceOrderResult) {
  const auto raw_json = R"(
{
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
      .transaction_time = 1648412181820,
      .type = stonks::binance::Type::kLimit};

  const auto parsed_json = web::json::value::parse(raw_json);
  const auto parsed_object =
      stonks::ParseFromJson<stonks::binance::PlaceOrderResult>(parsed_json);

  ASSERT_TRUE(parsed_object.has_value());
  EXPECT_EQ(*parsed_object, object);
}

#undef _TURN_OFF_PLATFORM_STRING