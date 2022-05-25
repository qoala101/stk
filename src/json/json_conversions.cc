#include "json_conversions.h"

#include <cpprest/json.h>
#include <spdlog/spdlog.h>

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <magic_enum.hpp>
#include <optional>
#include <stdexcept>

#include "finance_types.h"
#include "utils.h"

namespace stonks::json {
namespace {
template <typename T, typename Index>
[[nodiscard]] auto ParseJsonElement(const web::json::value& json, Index index) {
  return ParseFromJson<T>(json.at(index));
}

template <typename T, typename Index>
[[nodiscard]] auto GetJsonElementNoThrow(const web::json::value& json,
                                         Index index) -> web::json::value {
  try {
    return json.at(index);
  } catch (const std::exception& e) {
  }

  return web::json::value::null();
}

template <typename T, typename Index>
[[nodiscard]] auto ParseJsonElementNoThrow(const web::json::value& json,
                                           Index index) {
  return ParseFromJsonNoThrow<T>(GetJsonElementNoThrow<T>(json, index));
}
}  // namespace

template <>
auto ParseFromJson(const web::json::value& json) -> bool {
  return json.as_bool();
}

auto ConvertToJson(const bool& data) -> web::json::value {
  return web::json::value::boolean(data);
}

template <>
auto ParseFromJson(const web::json::value& json) -> int {
  return json.as_number().to_int32();
}

template <>
auto ParseFromJson(const web::json::value& json) -> int64_t {
  return json.as_number().to_int64();
}

template <>
auto ParseFromJson(const web::json::value& json) -> double {
  if (json.is_number()) {
    return json.as_number().to_double();
  }

  return std::stod(json.as_string());
}

template <>
auto ParseFromJson(const web::json::value& json) -> std::string {
  return json.as_string();
}

auto ConvertToJson(const std::string& data) -> web::json::value {
  return web::json::value::string(data);
}

auto ConvertToJson(std::string_view data) -> web::json::value {
  return web::json::value::string(std::string{data});
}

auto ConvertToJson(const char* data) -> web::json::value {
  return web::json::value::string(data);
}

template <>
auto ParseFromJson(const web::json::value& json) -> std::chrono::milliseconds {
  return std::chrono::milliseconds{json.as_number().to_int64()};
}

auto ConvertToJson(std::chrono::milliseconds data) -> web::json::value {
  return web::json::value::number(data.count());
}

template <>
auto ParseFromJson(const web::json::value& json) -> boost::uuids::uuid {
  return utils::ParseUuidFromString(json.as_string());
}

auto ConvertToJson(boost::uuids::uuid data) -> web::json::value {
  return web::json::value::string(utils::ConvertUuidToString(data));
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> binance::PlaceOrderAcknowledgement {
  return binance::PlaceOrderAcknowledgement{
      .symbol = ParseJsonElement<std::string>(json, "symbol"),
      .order_id = ParseJsonElement<int64_t>(json, "orderId"),
      .order_list_id = ParseJsonElement<int64_t>(json, "orderListId"),
      .client_order_id = ParseJsonElement<std::string>(json, "clientOrderId"),
      .transaction_time =
          ParseJsonElement<std::chrono::milliseconds>(json, "transactTime")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::ApiError {
  return binance::ApiError{
      .code = ParseJsonElement<int64_t>(json, "code"),
      .message = ParseJsonElement<std::string>(json, "msg")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::OrderInfo {
  return binance::OrderInfo{
      .symbol = ParseJsonElement<std::string>(json, "symbol"),
      .order_id = ParseJsonElement<int64_t>(json, "orderId"),
      .order_list_id = ParseJsonElement<int64_t>(json, "orderListId"),
      .client_order_id = ParseJsonElement<std::string>(json, "clientOrderId"),
      .price = ParseJsonElement<double>(json, "price"),
      .original_quantity = ParseJsonElement<double>(json, "origQty"),
      .executed_quantity = ParseJsonElement<double>(json, "executedQty"),
      .cummulative_quote_quantity =
          ParseJsonElement<double>(json, "cummulativeQuoteQty"),
      .status = ParseJsonElement<binance::OrderStatus>(json, "status"),
      .time_in_force =
          ParseJsonElement<binance::OrderTimeInForce>(json, "timeInForce"),
      .type = ParseJsonElement<binance::OrderType>(json, "type"),
      .side = ParseJsonElement<binance::OrderSide>(json, "side"),
      .stop_price = ParseJsonElement<double>(json, "stopPrice"),
      .iceberg_quantity = ParseJsonElement<double>(json, "icebergQty"),
      .time = ParseJsonElement<std::chrono::milliseconds>(json, "time"),
      .update_time =
          ParseJsonElement<std::chrono::milliseconds>(json, "updateTime"),
      .is_working = ParseJsonElement<bool>(json, "isWorking"),
      .original_quote_order_quantity =
          ParseJsonElement<double>(json, "origQuoteOrderQty")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::Kline {
  return binance::Kline{
      .open_time = ParseJsonElement<std::chrono::milliseconds>(json, 0),
      .open_price = ParseJsonElement<double>(json, 1),
      .high_price = ParseJsonElement<double>(json, 2),
      .low_price = ParseJsonElement<double>(json, 3),
      .close_price = ParseJsonElement<double>(json, 4),
      .volume = ParseJsonElement<double>(json, 5),
      .close_time = ParseJsonElement<std::chrono::milliseconds>(json, 6),
      .quote_asset_volume = ParseJsonElement<double>(json, 7),
      .num_trades = ParseJsonElement<int64_t>(json, 8),
      .taker_buy_base_asset_volume = ParseJsonElement<double>(json, 9),
      .taker_buy_quote_asset_volume = ParseJsonElement<double>(json, 10)};
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> binance::AverageSymbolPrice {
  return binance::AverageSymbolPrice{
      .mins = ParseJsonElement<int64_t>(json, "mins"),
      .price = ParseJsonElement<double>(json, "price")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::SymbolPrice {
  return binance::SymbolPrice{
      .symbol = ParseJsonElement<std::string>(json, "symbol"),
      .price = ParseJsonElement<double>(json, "price")};
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> binance::SymbolExchangeInfo {
  auto data = binance::SymbolExchangeInfo{
      .symbol = ParseJsonElement<std::string>(json, "symbol"),
      .base_asset = ParseJsonElement<std::string>(json, "baseAsset"),
      .quote_asset = ParseJsonElement<std::string>(json, "quoteAsset")};

  const auto& filters = json.at("filters").as_array();

  for (const auto& filter : filters) {
    const auto type = ParseJsonElement<std::string>(filter, "filterType");

    if (type == "LOT_SIZE") {
      data.min_quantity = ParseJsonElement<double>(filter, "minQty");
      data.step_size = ParseJsonElement<double>(filter, "stepSize");
      continue;
    }

    if (type == "MIN_NOTIONAL") {
      data.min_notional = ParseJsonElement<double>(filter, "minNotional");
      continue;
    }

    if (type == "PRICE_FILTER") {
      data.tick_size = ParseJsonElement<double>(filter, "tickSize");
      continue;
    }
  }

  return data;
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::ExchangeInfo {
  return binance::ExchangeInfo{
      .symbols = ParseJsonElement<std::vector<binance::SymbolExchangeInfo>>(
          json, "symbols")};
}

auto ConvertToJson(const binance::ExchangeInfo& /*unused*/)
    -> web::json::value {
  throw std::runtime_error{"Not implemented"};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::SymbolBookTicker {
  return binance::SymbolBookTicker{
      .order_book_update_id = ParseJsonElement<int64_t>(json, "u"),
      .symbol = ParseJsonElement<std::string>(json, "s"),
      .best_bid_price = ParseJsonElement<double>(json, "b"),
      .best_bid_quantity = ParseJsonElement<double>(json, "B"),
      .best_ask_price = ParseJsonElement<double>(json, "a"),
      .best_ask_quantity = ParseJsonElement<double>(json, "A")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::Symbol {
  return finance::Symbol{
      .base_asset = ParseJsonElement<std::string>(json, "base_asset"),
      .quote_asset = ParseJsonElement<std::string>(json, "quote_asset")};
}

auto ConvertToJson(const finance::Symbol& data) -> web::json::value {
  auto json = web::json::value{};
  json["base_asset"] = ConvertToJson(data.base_asset);
  json["quote_asset"] = ConvertToJson(data.quote_asset);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::TimeDouble {
  return finance::TimeDouble{
      .time = ParseJsonElement<std::chrono::milliseconds>(json, "time"),
      .value = ParseJsonElement<double>(json, "value")};
}

auto ConvertToJson(const finance::TimeDouble& data) -> web::json::value {
  auto json = web::json::value{};
  json["time"] = ConvertToJson(data.time);
  json["value"] = ConvertToJson(data.value);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::SymbolPrices {
  return finance::SymbolPrices{
      .symbol = ParseJsonElement<finance::Symbol>(json, "symbol"),
      .prices =
          ParseJsonElement<std::vector<finance::TimeDouble>>(json, "prices")};
}

auto ConvertToJson(const finance::SymbolPrices& data) -> web::json::value {
  auto json = web::json::value{};
  json["symbol"] = ConvertToJson(data.symbol);
  json["prices"] = ConvertToJson(data.prices);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::SymbolPriceTick {
  return finance::SymbolPriceTick{
      .symbol = ParseJsonElement<finance::SymbolName>(json, "symbol"),
      .time = ParseJsonElement<std::chrono::milliseconds>(json, "time"),
      .buy_price = ParseJsonElement<double>(json, "buy_price"),
      .sell_price = ParseJsonElement<double>(json, "sell_price")};
}

auto ConvertToJson(const finance::SymbolPriceTick& data) -> web::json::value {
  auto json = web::json::value{};
  json["symbol"] = ConvertToJson(data.symbol);
  json["time"] = ConvertToJson(data.time);
  json["buy_price"] = ConvertToJson(data.buy_price);
  json["sell_price"] = ConvertToJson(data.sell_price);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::StrategyData {
  const auto type = ParseJsonElement<std::string>(json, "typename");

  if (type == "BreakoutStrategyData") {
    return finance::StrategyData{
        .strategy_data = finance::BreakoutStrategyData{
            .last_candle_close_time =
                ParseJsonElement<std::chrono::milliseconds>(
                    json, "last_candle_close_time"),
            .expected_price =
                ParseJsonElement<double>(json, "expected_price")}};
  }

  if (type == "MeanAverageStrategyData") {
    return finance::StrategyData{.strategy_data =
                                     finance::MeanAverageStrategyData{}};
  }

  throw std::runtime_error{"Unknown typename"};
}

auto ConvertToJson(const finance::StrategyData& data) -> web::json::value {
  auto json = web::json::value{};
  std::visit(
      [&json](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, finance::BreakoutStrategyData>) {
          json["typename"] = ConvertToJson("BreakoutStrategyData");
          json["last_candle_close_time"] =
              ConvertToJson(variant.last_candle_close_time);
          json["expected_price"] = ConvertToJson(variant.expected_price);
          return;
        }

        if constexpr (std::is_same_v<T, finance::MeanAverageStrategyData>) {
          json["typename"] = ConvertToJson("MeanAverageStrategyData");
          return;
        }

        throw std::runtime_error{"Unknown type"};
      },
      data.strategy_data);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::OrderType {
  const auto type = ParseJsonElement<std::string>(json, "typename");

  if (type == "MarketOrderType") {
    return finance::OrderType{.order_type = finance::MarketOrderType{}};
  }

  if (type == "LimitOrderType") {
    return finance::OrderType{
        .order_type = finance::LimitOrderType{
            .price = ParseJsonElement<double>(json, "price")}};
  }

  throw std::runtime_error{"Unknown typename"};
}

auto ConvertToJson(const finance::OrderType& data) -> web::json::value {
  auto json = web::json::value{};
  std::visit(
      [&json](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, finance::LimitOrderType>) {
          json["typename"] = ConvertToJson("LimitOrderType");
          json["price"] = ConvertToJson(variant.price);
          return;
        }

        if constexpr (std::is_same_v<T, finance::MarketOrderType>) {
          json["typename"] = ConvertToJson("MarketOrderType");
          return;
        }

        throw std::runtime_error{"Unknown type"};
      },
      data.order_type);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::OrderUpdate {
  const auto type = ParseJsonElement<std::string>(json, "typename");

  if (type == "OrderError") {
    return finance::OrderUpdate{
        .order_update = finance::OrderError{
            .message = ParseJsonElement<std::string>(json, "message")}};
  }

  if (type == "OrderInfo") {
    return finance::OrderUpdate{
        .order_update = finance::OrderInfo{
            .order_status =
                ParseJsonElement<finance::OrderStatus>(json, "order_status"),
            .requested_amount =
                ParseJsonElement<double>(json, "requested_amount"),
            .executed_amount =
                ParseJsonElement<double>(json, "executed_amount"),
            .price = ParseJsonElement<double>(json, "price"),
            .executed_quote_amount =
                ParseJsonElement<double>(json, "executed_quote_amount")}};
  }

  throw std::runtime_error{"Unknown typename"};
}

auto ConvertToJson(const finance::OrderUpdate& data) -> web::json::value {
  auto json = web::json::value{};
  std::visit(
      [&json](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, finance::OrderError>) {
          json["typename"] = ConvertToJson("OrderError");
          json["message"] = ConvertToJson(variant.message);
          return;
        }

        if constexpr (std::is_same_v<T, finance::OrderInfo>) {
          json["typename"] = ConvertToJson("OrderInfo");
          json["order_status"] = ConvertToJson(variant.order_status);
          json["requested_amount"] = ConvertToJson(variant.requested_amount);
          json["executed_amount"] = ConvertToJson(variant.executed_amount);
          json["price"] = ConvertToJson(variant.price);
          json["executed_quote_amount"] =
              ConvertToJson(variant.executed_quote_amount);
          return;
        }

        throw std::runtime_error{"Unknown type"};
      },
      data.order_update);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::OrderProxyOrderUpdate {
  return finance::OrderProxyOrderUpdate{
      .received_time =
          ParseJsonElement<std::chrono::milliseconds>(json, "received_time"),
      .order_update =
          ParseJsonElement<finance::OrderUpdate>(json, "last_order_update")};
}

auto ConvertToJson(const finance::OrderProxyOrderUpdate& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["received_time"] = ConvertToJson(data.received_time);
  json["order_update"] = ConvertToJson(data.order_update);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::Amount {
  const auto type = ParseJsonElement<std::string>(json, "typename");

  if (type == "UnspecifiedAmount") {
    return finance::Amount{.amount = finance::UnspecifiedAmount{}};
  }

  if (type == "BaseAmount") {
    return finance::Amount{
        .amount = finance::BaseAmount{
            .base_amount = ParseJsonElement<double>(json, "base_amount")}};
  }

  if (type == "QuoteAmount") {
    return finance::Amount{
        .amount = finance::QuoteAmount{
            .quote_amount = ParseJsonElement<double>(json, "quote_amount")}};
  }

  throw std::runtime_error{"Unknown typename"};
}

auto ConvertToJson(const finance::Amount& data) -> web::json::value {
  auto json = web::json::value{};
  std::visit(
      [&json](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, finance::UnspecifiedAmount>) {
          json["typename"] = ConvertToJson("UnspecifiedAmount");
          return;
        }

        if constexpr (std::is_same_v<T, finance::BaseAmount>) {
          json["typename"] = ConvertToJson("BaseAmount");
          json["base_amount"] = ConvertToJson(variant.base_amount);
          return;
        }

        if constexpr (std::is_same_v<T, finance::QuoteAmount>) {
          json["typename"] = ConvertToJson("QuoteAmount");
          json["quote_amount"] = ConvertToJson(variant.quote_amount);
          return;
        }

        throw std::runtime_error{"Unknown type"};
      },
      data.amount);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::StrategyOrderRequest {
  return finance::StrategyOrderRequest{
      .order_uuid = ParseJsonElement<boost::uuids::uuid>(json, "order_uuid"),
      .symbol = ParseJsonElement<finance::Symbol>(json, "symbol"),
      .buy_or_sell = ParseJsonElement<finance::BuyOrSell>(json, "buy_or_sell"),
      .amount = ParseJsonElement<finance::Amount>(json, "amount"),
      .order_type = ParseJsonElement<finance::OrderType>(json, "order_type"),
      .strategy_data =
          ParseJsonElement<finance::StrategyData>(json, "strategy_data")};
}

auto ConvertToJson(const finance::StrategyOrderRequest& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["symbol"] = ConvertToJson(data.symbol);
  json["buy_or_sell"] = ConvertToJson(data.buy_or_sell);
  json["amount"] = ConvertToJson(data.amount);
  json["order_type"] = ConvertToJson(data.order_type);
  json["strategy_data"] = ConvertToJson(data.strategy_data);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::OrderProxyOrderRequest {
  return finance::OrderProxyOrderRequest{
      .order_uuid = ParseJsonElement<boost::uuids::uuid>(json, "order_uuid"),
      .symbol = ParseJsonElement<finance::Symbol>(json, "symbol"),
      .buy_or_sell = ParseJsonElement<finance::BuyOrSell>(json, "buy_or_sell"),
      .amount = ParseJsonElement<finance::Amount>(json, "amount"),
      .order_type = ParseJsonElement<finance::OrderType>(json, "order_type")};
}

auto ConvertToJson(const finance::OrderProxyOrderRequest& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["symbol"] = ConvertToJson(data.symbol);
  json["buy_or_sell"] = ConvertToJson(data.buy_or_sell);
  json["amount"] = ConvertToJson(data.amount);
  json["order_type"] = ConvertToJson(data.order_type);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::StrategySubscribeToOrderUpdatesRequest {
  return finance::StrategySubscribeToOrderUpdatesRequest{
      .order_uuid = ParseJsonElement<boost::uuids::uuid>(json, "order_uuid"),
      .subscriber_uri = ParseJsonElement<std::string>(json, "subscriber_uri")};
}

auto ConvertToJson(const finance::StrategySubscribeToOrderUpdatesRequest& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["subscriber_uri"] = ConvertToJson(data.subscriber_uri);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::PriceTicksServiceSubscribeRequest {
  return finance::PriceTicksServiceSubscribeRequest{
      .subscriber_uri = ParseJsonElement<std::string>(json, "subscriber_uri")};
}

auto ConvertToJson(const finance::PriceTicksServiceSubscribeRequest& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["subscriber_uri"] = ConvertToJson(data.subscriber_uri);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::OrderProxyMonitorRequest {
  return finance::OrderProxyMonitorRequest{
      .order_uuid = ParseJsonElement<boost::uuids::uuid>(json, "order_uuid"),
      .symbol = ParseJsonElement<finance::Symbol>(json, "symbol"),
      .last_order_update = ParseJsonElementNoThrow<finance::OrderUpdate>(
          json, "last_order_update")};
}

auto ConvertToJson(const finance::OrderProxyMonitorRequest& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["symbol"] = ConvertToJson(data.symbol);

  if (data.last_order_update.has_value()) {
    json["last_order_update"] = ConvertToJson(*data.last_order_update);
  }

  return json;
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> finance::OrderMonitorOrderUpdate {
  return finance::OrderMonitorOrderUpdate{
      .order_uuid = ParseJsonElement<boost::uuids::uuid>(json, "order_uuid"),
      .order_update =
          ParseJsonElement<finance::OrderUpdate>(json, "order_update")};
}

auto ConvertToJson(const finance::OrderMonitorOrderUpdate& data)
    -> web::json::value {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["order_update"] = ConvertToJson(data.order_update);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> std::runtime_error {
  const auto type = ParseJsonElement<std::string>(json, "typename");

  if (type == "runtime_error") {
    return std::runtime_error{ParseJsonElement<std::string>(json, "message")};
  }

  throw std::runtime_error{"Unknown typename"};
}

auto ConvertToJson(const std::runtime_error& data) -> web::json::value {
  auto json = web::json::value{};
  json["typename"] = ConvertToJson("runtime_error");
  json["message"] = ConvertToJson(data.what());
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::Period {
  return finance::Period{
      .start_time = ParseJsonElementNoThrow<std::chrono::milliseconds>(
          json, "start_time"),
      .end_time =
          ParseJsonElementNoThrow<std::chrono::milliseconds>(json, "end_time")};
}

auto ConvertToJson(const finance::Period& data) -> web::json::value {
  auto json = web::json::value{};
  json["start_time"] = ConvertToJson(data.start_time);
  json["end_time"] = ConvertToJson(data.end_time);
  return json;
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::SymbolInfo {
  return finance::SymbolInfo{
      .symbol = ParseJsonElement<std::string>(json, "symbol"),
      .base_asset = ParseJsonElement<std::string>(json, "base_asset"),
      .quote_asset = ParseJsonElement<std::string>(json, "quote_asset"),
      .min_base_amount = ParseJsonElement<double>(json, "min_base_amount"),
      .min_quote_amount = ParseJsonElement<double>(json, "min_quote_amount"),
      .base_step = ParseJsonElement<double>(json, "base_step"),
      .quote_step = ParseJsonElement<double>(json, "quote_step")};
}

auto ConvertToJson(const finance::SymbolInfo& data) -> web::json::value {
  auto json = web::json::value{};
  json["symbol"] = ConvertToJson(data.symbol);
  json["base_asset"] = ConvertToJson(data.base_asset);
  json["quote_asset"] = ConvertToJson(data.quote_asset);
  json["min_base_amount"] = ConvertToJson(data.min_base_amount);
  json["min_quote_amount"] = ConvertToJson(data.min_quote_amount);
  json["base_step"] = ConvertToJson(data.base_step);
  json["quote_step"] = ConvertToJson(data.quote_step);
  return json;
}
}  // namespace stonks::json