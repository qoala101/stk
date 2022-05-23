#include "json_conversions.h"

#include <spdlog/spdlog.h>

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <magic_enum.hpp>
#include <optional>
#include <stdexcept>

#include "utils.h"

namespace stonks::json {
namespace {
auto GetStringPropertyAsString(const web::json::value& json,
                               const std::string_view property_name)
    -> const std::string& {
  return json.at(std::string{property_name}).as_string();
}
auto GetStringElementAsString(const web::json::value& json, const int index)
    -> const std::string& {
  return json.at(index).as_string();
}

auto GetStringPropertyAsDouble(const web::json::value& json,
                               const std::string_view property_name) -> double {
  return std::stod(json.at(std::string{property_name}).as_string());
}

auto GetStringElementAsDouble(const web::json::value& json, const int index)
    -> double {
  return std::stod(json.at(index).as_string());
}

auto GetIntPropertyAsInt(const web::json::value& json,
                         const std::string_view property_name) -> int {
  return json.at(std::string{property_name}).as_number().to_int32();
}

auto GetIntElementAsInt(const web::json::value& json, const int index) -> int {
  return json.at(index).as_number().to_int32();
}

auto GetInt64PropertyAsInt64(const web::json::value& json,
                             const std::string_view property_name) -> int64_t {
  return json.at(std::string{property_name}).as_number().to_int64();
}

auto GetInt64ElementAsInt64(const web::json::value& json, const int index)
    -> int64_t {
  return json.at(index).as_number().to_int64();
}

auto GetDoublePropertyAsDouble(const web::json::value& json,
                               const std::string_view property_name) -> double {
  return json.at(std::string{property_name}).as_number().to_double();
}

auto GetDoubleElementAsDouble(const web::json::value& json, const int index)
    -> double {
  return json.at(index).as_number().to_double();
}

auto GetBoolPropertyAsBool(const web::json::value& json,
                           const std::string_view property_name) -> bool {
  return json.at(std::string{property_name}).as_bool();
}

auto GetBoolElementAsBool(const web::json::value& json, const int index)
    -> bool {
  return json.at(index).as_bool();
}

auto GetInt64PropertyAsMilliseconds(const web::json::value& json,
                                    const std::string_view property_name)
    -> std::chrono::milliseconds {
  return std::chrono::milliseconds{
      json.at(std::string{property_name}).as_number().to_int64()};
}

auto GetInt64PropertyAsOptionalMilliseconds(
    const web::json::value& json, const std::string_view property_name)
    -> std::optional<std::chrono::milliseconds> {
  const auto property_name_string = std::string{property_name};

  if (!json.has_field(property_name_string)) {
    return std::nullopt;
  }

  const auto& value = json.at(property_name_string);

  if (value.is_null()) {
    return std::nullopt;
  }

  return std::chrono::milliseconds{value.as_number().to_int64()};
}

auto GetInt64ElementAsMilliseconds(const web::json::value& json,
                                   const int index)
    -> std::chrono::milliseconds {
  return std::chrono::milliseconds{json.at(index).as_number().to_int64()};
}

auto GetStringPropertyAsUuid(const web::json::value& json,
                             const std::string_view property_name)
    -> boost::uuids::uuid {
  return utils::ParseUuidFromString(
      json.at(std::string{property_name}).as_string());
}

auto GetStringElementAsUuid(const web::json::value& json, const int index)
    -> boost::uuids::uuid {
  return utils::ParseUuidFromString(json.at(index).as_string());
}

template <Enumeration E>
auto GetStringPropertyAsEnum(const web::json::value& json,
                             const std::string_view property_name) -> E {
  return magic_enum::enum_cast<E>(
             json.at(std::string{property_name}).as_string())
      .value_or(E::kInvalid);
}

template <Enumeration E>
auto GetStringElementAsEnum(const web::json::value& json, const int index)
    -> E {
  return magic_enum::enum_cast<E>(json.at(index).as_string())
      .value_or(E::kInvalid);
}

template <typename T>
auto GetObjectPropertyAsObject(const web::json::value& json,
                               const std::string_view property_name) -> T {
  return ParseFromJson<T>(json.at(std::string{property_name}));
}

template <typename T>
auto GetObjectPropertyAsOptionalObject(const web::json::value& json,
                                       const std::string_view property_name)
    -> std::optional<T> {
  const auto property_name_string = std::string{property_name};

  if (!json.has_field(property_name_string)) {
    return std::nullopt;
  }

  const auto& value = json.at(property_name_string);

  if (value.is_null()) {
    return std::nullopt;
  }

  return ParseFromJson<T>(json.at(std::string{property_name}));
}

template <typename T>
auto GetObjectElementAsObject(const web::json::value& json, const int index)
    -> T {
  return ParseFromJson<T>(json.at(index));
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
  return json.as_number().to_double();
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

auto ConvertToJson(boost::uuids::uuid data) -> web::json::value {
  return web::json::value::string(utils::ConvertUuidToString(data));
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> binance::PlaceOrderAcknowledgement {
  return binance::PlaceOrderAcknowledgement{
      .symbol = GetStringPropertyAsString(json, "symbol"),
      .order_id = GetInt64PropertyAsInt64(json, "orderId"),
      .order_list_id = GetInt64PropertyAsInt64(json, "orderListId"),
      .client_order_id = GetStringPropertyAsString(json, "clientOrderId"),
      .transaction_time = GetInt64PropertyAsMilliseconds(json, "transactTime")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::ApiError {
  return binance::ApiError{.code = GetInt64PropertyAsInt64(json, "code"),
                           .message = GetStringPropertyAsString(json, "msg")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::OrderInfo {
  return binance::OrderInfo{
      .symbol = GetStringPropertyAsString(json, "symbol"),
      .order_id = GetInt64PropertyAsInt64(json, "orderId"),
      .order_list_id = GetInt64PropertyAsInt64(json, "orderListId"),
      .client_order_id = GetStringPropertyAsString(json, "clientOrderId"),
      .price = GetStringPropertyAsDouble(json, "price"),
      .original_quantity = GetStringPropertyAsDouble(json, "origQty"),
      .executed_quantity = GetStringPropertyAsDouble(json, "executedQty"),
      .cummulative_quote_quantity =
          GetStringPropertyAsDouble(json, "cummulativeQuoteQty"),
      .status = GetStringPropertyAsEnum<binance::OrderStatus>(json, "status"),
      .time_in_force = GetStringPropertyAsEnum<binance::OrderTimeInForce>(
          json, "timeInForce"),
      .type = GetStringPropertyAsEnum<binance::OrderType>(json, "type"),
      .side = GetStringPropertyAsEnum<binance::OrderSide>(json, "side"),
      .stop_price = GetStringPropertyAsDouble(json, "stopPrice"),
      .iceberg_quantity = GetStringPropertyAsDouble(json, "icebergQty"),
      .time = GetInt64PropertyAsMilliseconds(json, "time"),
      .update_time = GetInt64PropertyAsMilliseconds(json, "updateTime"),
      .is_working = GetBoolPropertyAsBool(json, "isWorking"),
      .original_quote_order_quantity =
          GetStringPropertyAsDouble(json, "origQuoteOrderQty")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::Kline {
  return binance::Kline{
      .open_time = GetInt64ElementAsMilliseconds(json, 0),
      .open_price = GetStringElementAsDouble(json, 1),
      .high_price = GetStringElementAsDouble(json, 2),
      .low_price = GetStringElementAsDouble(json, 3),
      .close_price = GetStringElementAsDouble(json, 4),
      .volume = GetStringElementAsDouble(json, 5),
      .close_time = GetInt64ElementAsMilliseconds(json, 6),
      .quote_asset_volume = GetStringElementAsDouble(json, 7),
      .num_trades = GetInt64ElementAsInt64(json, 8),
      .taker_buy_base_asset_volume = GetStringElementAsDouble(json, 9),
      .taker_buy_quote_asset_volume = GetStringElementAsDouble(json, 10)};
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> binance::AverageSymbolPrice {
  return binance::AverageSymbolPrice{
      .mins = GetInt64PropertyAsInt64(json, "mins"),
      .price = GetStringPropertyAsDouble(json, "price")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::SymbolPrice {
  return binance::SymbolPrice{
      .symbol = GetStringPropertyAsString(json, "symbol"),
      .price = GetStringPropertyAsDouble(json, "price")};
}

template <>
auto ParseFromJson(const web::json::value& json)
    -> binance::SymbolExchangeInfo {
  return binance::SymbolExchangeInfo{
      .base_asset = GetStringPropertyAsString(json, "baseAsset"),
      .quote_asset = GetStringPropertyAsString(json, "quoteAsset")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::ExchangeInfo {
  return binance::ExchangeInfo{
      .symbols =
          GetObjectPropertyAsObject<std::vector<binance::SymbolExchangeInfo>>(
              json, "symbols")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> binance::SymbolBookTicker {
  return binance::SymbolBookTicker{
      .order_book_update_id = GetInt64PropertyAsInt64(json, "u"),
      .symbol = GetStringPropertyAsString(json, "s"),
      .best_bid_price = GetStringPropertyAsDouble(json, "b"),
      .best_bid_quantity = GetStringPropertyAsDouble(json, "B"),
      .best_ask_price = GetStringPropertyAsDouble(json, "a"),
      .best_ask_quantity = GetStringPropertyAsDouble(json, "A")};
}

template <>
auto ParseFromJson(const web::json::value& json) -> finance::Symbol {
  return finance::Symbol{
      .base_asset = GetStringPropertyAsString(json, "base_asset"),
      .quote_asset = GetStringPropertyAsString(json, "quote_asset")};
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
      .time = GetInt64PropertyAsMilliseconds(json, "time"),
      .value = GetDoublePropertyAsDouble(json, "value")};
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
      .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
      .prices = GetObjectPropertyAsObject<std::vector<finance::TimeDouble>>(
          json, "prices")};
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
      .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
      .time = GetInt64PropertyAsMilliseconds(json, "time"),
      .buy_price = GetDoublePropertyAsDouble(json, "buy_price"),
      .sell_price = GetDoublePropertyAsDouble(json, "sell_price")};
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
  const auto type = GetStringPropertyAsString(json, "typename");

  if (type == "BreakoutStrategyData") {
    return finance::StrategyData{
        .strategy_data = finance::BreakoutStrategyData{
            .last_candle_close_time =
                GetInt64PropertyAsMilliseconds(json, "last_candle_close_time"),
            .expected_price =
                GetDoublePropertyAsDouble(json, "expected_price")}};
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
  const auto type = GetStringPropertyAsString(json, "typename");

  if (type == "MarketOrderType") {
    return finance::OrderType{.order_type = finance::MarketOrderType{}};
  }

  if (type == "LimitOrderType") {
    return finance::OrderType{
        .order_type = finance::LimitOrderType{
            .price = GetDoublePropertyAsDouble(json, "price")}};
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
  const auto type = GetStringPropertyAsString(json, "typename");

  if (type == "OrderError") {
    return finance::OrderUpdate{
        .order_update = finance::OrderError{
            .message = GetStringPropertyAsString(json, "message")}};
  }

  if (type == "OrderInfo") {
    return finance::OrderUpdate{
        .order_update = finance::OrderInfo{
            .order_status = GetStringPropertyAsEnum<finance::OrderStatus>(
                json, "order_status"),
            .requested_amount =
                GetDoublePropertyAsDouble(json, "requested_amount"),
            .executed_amount =
                GetDoublePropertyAsDouble(json, "executed_amount"),
            .price = GetDoublePropertyAsDouble(json, "price"),
            .executed_quote_amount =
                GetDoublePropertyAsDouble(json, "executed_quote_amount")}};
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
      .received_time = GetInt64PropertyAsMilliseconds(json, "received_time"),
      .order_update = GetObjectPropertyAsObject<finance::OrderUpdate>(
          json, "last_order_update")};
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
  const auto type = GetStringPropertyAsString(json, "typename");

  if (type == "UnspecifiedAmount") {
    return finance::Amount{.amount = finance::UnspecifiedAmount{}};
  }

  if (type == "BaseAmount") {
    return finance::Amount{
        .amount = finance::BaseAmount{
            .base_amount = GetDoublePropertyAsDouble(json, "base_amount")}};
  }

  if (type == "QuoteAmount") {
    return finance::Amount{
        .amount = finance::QuoteAmount{
            .quote_amount = GetDoublePropertyAsDouble(json, "quote_amount")}};
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
      .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
      .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
      .buy_or_sell =
          GetStringPropertyAsEnum<finance::BuyOrSell>(json, "buy_or_sell"),
      .amount = GetObjectPropertyAsObject<finance::Amount>(json, "amount"),
      .order_type =
          GetObjectPropertyAsObject<finance::OrderType>(json, "order_type"),
      .strategy_data = GetObjectPropertyAsObject<finance::StrategyData>(
          json, "strategy_data")};
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
      .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
      .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
      .buy_or_sell =
          GetStringPropertyAsEnum<finance::BuyOrSell>(json, "buy_or_sell"),
      .amount = GetObjectPropertyAsObject<finance::Amount>(json, "amount"),
      .order_type =
          GetObjectPropertyAsObject<finance::OrderType>(json, "order_type")};
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
      .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
      .subscriber_uri = GetStringPropertyAsString(json, "subscriber_uri")};
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
      .subscriber_uri = GetStringPropertyAsString(json, "subscriber_uri")};
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
      .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
      .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
      .last_order_update =
          GetObjectPropertyAsOptionalObject<finance::OrderUpdate>(
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
      .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
      .order_update = GetObjectPropertyAsObject<finance::OrderUpdate>(
          json, "order_update")};
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
  const auto type = GetStringPropertyAsString(json, "typename");

  if (type == "runtime_error") {
    return std::runtime_error{GetStringPropertyAsString(json, "message")};
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
      .start_time = GetInt64PropertyAsOptionalMilliseconds(json, "start_time"),
      .end_time = GetInt64PropertyAsOptionalMilliseconds(json, "end_time")};
}

auto ConvertToJson(const finance::Period& data) -> web::json::value {
  auto json = web::json::value{};
  json["start_time"] = ConvertToJson(data.start_time);
  json["end_time"] = ConvertToJson(data.end_time);
  return json;
}
}  // namespace stonks::json