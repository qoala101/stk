#include "json_conversions.h"

#include <spdlog/spdlog.h>

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <magic_enum.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_enum_conversions.h"
#include "concepts.h"
#include "utils.h"

namespace stonks::json {
namespace {
const std::string& GetStringPropertyAsString(
    const web::json::value& json, const std::string_view property_name) {
  return json.at(std::string{property_name}).as_string();
}
const std::string& GetStringElementAsString(const web::json::value& json,
                                            const int index) {
  return json.at(index).as_string();
}

double GetStringPropertyAsDouble(const web::json::value& json,
                                 const std::string_view property_name) {
  return std::stod(json.at(std::string{property_name}).as_string());
}

double GetStringElementAsDouble(const web::json::value& json, const int index) {
  return std::stod(json.at(index).as_string());
}

int GetIntPropertyAsInt(const web::json::value& json,
                        const std::string_view property_name) {
  return json.at(std::string{property_name}).as_number().to_int32();
}

int GetIntElementAsInt(const web::json::value& json, const int index) {
  return json.at(index).as_number().to_int32();
}

int64_t GetInt64PropertyAsInt64(const web::json::value& json,
                                const std::string_view property_name) {
  return json.at(std::string{property_name}).as_number().to_int64();
}

int64_t GetInt64ElementAsInt64(const web::json::value& json, const int index) {
  return json.at(index).as_number().to_int64();
}

double GetDoublePropertyAsDouble(const web::json::value& json,
                                 const std::string_view property_name) {
  return json.at(std::string{property_name}).as_number().to_double();
}

double GetDoubleElementAsDouble(const web::json::value& json, const int index) {
  return json.at(index).as_number().to_double();
}

bool GetBoolPropertyAsBool(const web::json::value& json,
                           const std::string_view property_name) {
  return json.at(std::string{property_name}).as_bool();
}

bool GetBoolElementAsBool(const web::json::value& json, const int index) {
  return json.at(index).as_bool();
}

std::chrono::milliseconds GetInt64PropertyAsMilliseconds(
    const web::json::value& json, const std::string_view property_name) {
  return std::chrono::milliseconds{
      json.at(std::string{property_name}).as_number().to_int64()};
}

std::chrono::milliseconds GetInt64ElementAsMilliseconds(
    const web::json::value& json, const int index) {
  return std::chrono::milliseconds{json.at(index).as_number().to_int64()};
}

boost::uuids::uuid GetStringPropertyAsUuid(
    const web::json::value& json, const std::string_view property_name) {
  return utils::ParseUuidFromString(
      json.at(std::string{property_name}).as_string());
}

boost::uuids::uuid GetStringElementAsUuid(const web::json::value& json,
                                          const int index) {
  return utils::ParseUuidFromString(json.at(index).as_string());
}

template <Enumeration E>
E GetStringPropertyAsEnum(const web::json::value& json,
                          const std::string_view property_name) {
  return magic_enum::enum_cast<E>(
             json.at(std::string{property_name}).as_string())
      .value_or(E::kInvalid);
}

template <Enumeration E>
E GetStringElementAsEnum(const web::json::value& json, const int index) {
  return magic_enum::enum_cast<E>(json.at(index).as_string())
      .value_or(E::kInvalid);
}

template <typename T>
T GetObjectPropertyAsObject(const web::json::value& json,
                            const std::string_view property_name) {
  const auto object = ParseFromJson<T>(json.at(std::string{property_name}));

  if (!object.has_value()) {
    throw std::invalid_argument{"Cannot parse object"};
  }

  return *object;
}

template <typename T>
T GetObjectElementAsObject(const web::json::value& json, const int index) {
  const auto object = ParseFromJson<T>(json.at(index));

  if (!object.has_value()) {
    throw std::invalid_argument{"Cannot parse object"};
  }

  return *object;
}

template <typename T>
std::optional<T> ParseFromJsonArray(const web::json::value& json) {
  using V = typename T::value_type;

  if (!json.is_array()) {
    spdlog::error("Parse from JSON: {}", "not array");
    return std::nullopt;
  }

  const auto& json_array = json.as_array();

  const auto parse_item = [](const web::json::value& value) {
    return ParseFromJson<V>(value);
  };
  auto item_successfully_parsed = [](const std::optional<V>& item) {
    return item.has_value();
  };
  const auto get_item = [](const std::optional<V>& item) {
    return std::move(*item);
  };

  const auto parsed_items =
      json_array | ranges::views::transform(parse_item) |
      ranges::views::take_while(item_successfully_parsed) |
      ranges::views::transform(get_item) | ranges::to_vector;

  if (parsed_items.size() < json_array.size()) {
    spdlog::error("Parse from JSON: {}", "cannot parse all items");
    return std::nullopt;
  }

  return parsed_items;
}

template <typename T>
web::json::value ConvertToJsonArray(const std::vector<T>& data) {
  const auto convert_item = [](const T& item) { return ConvertToJson(item); };
  auto json_items =
      data | ranges::views::transform(convert_item) | ranges::to_vector;
  return web::json::value::array(std::move(json_items));
}
}  // namespace

web::json::value ConvertToJson(const std::string& data) {
  return web::json::value::string(data);
}

web::json::value ConvertToJson(std::string_view data) {
  return web::json::value::string(std::string{data});
}

web::json::value ConvertToJson(const char* data) {
  return web::json::value::string(data);
}

web::json::value ConvertToJson(std::chrono::milliseconds data) {
  return web::json::value::number(data.count());
}

web::json::value ConvertToJson(boost::uuids::uuid data) {
  return web::json::value::string(utils::ConvertUuidToString(data));
}

template <>
std::optional<binance::PlaceOrderAcknowledgement> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::PlaceOrderAcknowledgement{
        .symbol = GetStringPropertyAsString(json, "symbol"),
        .order_id = GetInt64PropertyAsInt64(json, "orderId"),
        .order_list_id = GetInt64PropertyAsInt64(json, "orderListId"),
        .client_order_id = GetStringPropertyAsString(json, "clientOrderId"),
        .transaction_time =
            GetInt64PropertyAsMilliseconds(json, "transactTime")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<binance::ApiError> ParseFromJson(const web::json::value& json) {
  try {
    return binance::ApiError{.code = GetInt64PropertyAsInt64(json, "code"),
                             .message = GetStringPropertyAsString(json, "msg")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<binance::OrderInfo> ParseFromJson(const web::json::value& json) {
  try {
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
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<std::vector<binance::OrderInfo>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<binance::OrderInfo>>(json);
}

template <>
std::optional<binance::Kline> ParseFromJson(const web::json::value& json) {
  try {
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
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<std::vector<binance::Kline>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<binance::Kline>>(json);
}

template <>
std::optional<binance::AverageSymbolPrice> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::AverageSymbolPrice{
        .mins = GetInt64PropertyAsInt64(json, "mins"),
        .price = GetStringPropertyAsDouble(json, "price")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<binance::SymbolPrice> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::SymbolPrice{
        .symbol = GetStringPropertyAsString(json, "symbol"),
        .price = GetStringPropertyAsDouble(json, "price")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<std::vector<binance::SymbolPrice>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<binance::SymbolPrice>>(json);
}

template <>
std::optional<binance::SymbolExchangeInfo> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::SymbolExchangeInfo{
        .base_asset = GetStringPropertyAsString(json, "baseAsset"),
        .quote_asset = GetStringPropertyAsString(json, "quoteAsset")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<std::vector<binance::SymbolExchangeInfo>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<binance::SymbolExchangeInfo>>(json);
}

template <>
std::optional<binance::ExchangeInfo> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::ExchangeInfo{
        .symbols =
            GetObjectPropertyAsObject<std::vector<binance::SymbolExchangeInfo>>(
                json, "symbols")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<finance::Symbol> ParseFromJson(const web::json::value& json) {
  try {
    return finance::Symbol{
        .base_asset = GetStringPropertyAsString(json, "base_asset"),
        .quote_asset = GetStringPropertyAsString(json, "quote_asset")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::Symbol& data) {
  auto json = web::json::value{};
  json["base_asset"] = ConvertToJson(data.base_asset);
  json["quote_asset"] = ConvertToJson(data.quote_asset);
  return json;
}

template <>
std::optional<finance::TimeDouble> ParseFromJson(const web::json::value& json) {
  try {
    return finance::TimeDouble{
        .time = GetInt64PropertyAsMilliseconds(json, "time"),
        .value = GetDoublePropertyAsDouble(json, "value")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::TimeDouble& data) {
  auto json = web::json::value{};
  json["time"] = ConvertToJson(data.time);
  json["value"] = ConvertToJson(data.value);
  return json;
}

template <>
std::optional<std::vector<finance::TimeDouble>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<finance::TimeDouble>>(json);
}

web::json::value ConvertToJson(const std::vector<finance::TimeDouble>& data) {
  return ConvertToJsonArray(data);
}

template <>
std::optional<finance::SymbolPrices> ParseFromJson(
    const web::json::value& json) {
  try {
    return finance::SymbolPrices{
        .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
        .prices = GetObjectPropertyAsObject<std::vector<finance::TimeDouble>>(
            json, "prices")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::SymbolPrices& data) {
  auto json = web::json::value{};
  json["symbol"] = ConvertToJson(data.symbol);
  json["prices"] = ConvertToJson(data.prices);
  return json;
}

template <>
std::optional<std::vector<finance::SymbolPrices>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<finance::SymbolPrices>>(json);
}

web::json::value ConvertToJson(const std::vector<finance::SymbolPrices>& data) {
  return ConvertToJsonArray(data);
}

template <>
std::optional<finance::StrategyData> ParseFromJson(
    const web::json::value& json) {
  try {
    const auto type = GetStringPropertyAsString(json, "typename");

    if (type == "BreakoutStrategyData") {
      return finance::StrategyData{
          .strategy_data = finance::BreakoutStrategyData{
              .last_candle_close_time = GetInt64PropertyAsMilliseconds(
                  json, "last_candle_close_time"),
              .expected_price =
                  GetDoublePropertyAsDouble(json, "expected_price")}};
    }

    if (type == "MeanAverageStrategyData") {
      return finance::StrategyData{.strategy_data =
                                       finance::MeanAverageStrategyData{}};
    }
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::StrategyData& data) {
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
      },
      data.strategy_data);
  return json;
}

template <>
std::optional<finance::OrderType> ParseFromJson(const web::json::value& json) {
  try {
    const auto type = GetStringPropertyAsString(json, "typename");

    if (type == "MarketOrderType") {
      return finance::OrderType{.order_type = finance::MarketOrderType{}};
    }

    if (type == "LimitOrderType") {
      return finance::OrderType{
          .order_type = finance::LimitOrderType{
              .price = GetDoublePropertyAsDouble(json, "price")}};
    }
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderType& data) {
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
      },
      data.order_type);
  return json;
}

template <>
std::optional<finance::OrderUpdate> ParseFromJson(
    const web::json::value& json) {
  try {
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
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderUpdate& data) {
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
      },
      data.order_update);
  return json;
}

template <>
std::optional<finance::OrderProxyOrderUpdate> ParseFromJson(
    const web::json::value& json) {
  try {
    return finance::OrderProxyOrderUpdate{
        .received_time = GetInt64PropertyAsMilliseconds(json, "received_time"),
        .order_update = GetObjectPropertyAsObject<finance::OrderUpdate>(
            json, "last_order_update")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderProxyOrderUpdate& data) {
  auto json = web::json::value{};
  json["received_time"] = ConvertToJson(data.received_time);
  json["order_update"] = ConvertToJson(data.order_update);
  return json;
}

template <>
std::optional<std::vector<finance::OrderProxyOrderUpdate>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<finance::OrderProxyOrderUpdate>>(json);
}

web::json::value ConvertToJson(
    const std::vector<finance::OrderProxyOrderUpdate>& data) {
  return ConvertToJsonArray(data);
}

template <>
std::optional<finance::Amount> ParseFromJson(const web::json::value& json) {
  try {
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
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::Amount& data) {
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
      },
      data.amount);
  return json;
}

// template <>
// std::optional<finance::Order> ParseFromJson(const web::json::value& json) {
//   return std::nullopt;
// }

// web::json::value ConvertToJson(const finance::Order& data) { return {}; }

template <>
std::optional<finance::StrategyOrderRequest> ParseFromJson(
    const web::json::value& json) {
  try {
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
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::StrategyOrderRequest& data) {
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
std::optional<finance::OrderProxyOrderRequest> ParseFromJson(
    const web::json::value& json) {
  try {
    return finance::OrderProxyOrderRequest{
        .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
        .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
        .buy_or_sell =
            GetStringPropertyAsEnum<finance::BuyOrSell>(json, "buy_or_sell"),
        .amount = GetObjectPropertyAsObject<finance::Amount>(json, "amount"),
        .order_type =
            GetObjectPropertyAsObject<finance::OrderType>(json, "order_type")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderProxyOrderRequest& data) {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["symbol"] = ConvertToJson(data.symbol);
  json["buy_or_sell"] = ConvertToJson(data.buy_or_sell);
  json["amount"] = ConvertToJson(data.amount);
  json["order_type"] = ConvertToJson(data.order_type);
  return json;
}

template <>
std::optional<finance::StrategySubscribeToOrderUpdatesRequest> ParseFromJson(
    const web::json::value& json) {
  try {
    return finance::StrategySubscribeToOrderUpdatesRequest{
        .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
        .subscriber_uri = GetStringPropertyAsString(json, "subscriber_uri")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(
    const finance::StrategySubscribeToOrderUpdatesRequest& data) {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["subscriber_uri"] = ConvertToJson(data.subscriber_uri);
  return json;
}

template <>
std::optional<finance::OrderProxyMonitorRequest> ParseFromJson(
    const web::json::value& json) {
  try {
    return finance::OrderProxyMonitorRequest{
        .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
        .symbol = GetObjectPropertyAsObject<finance::Symbol>(json, "symbol"),
        .last_order_update =
            json.has_field("last_order_update")
                ? GetObjectPropertyAsObject<finance::OrderUpdate>(
                      json, "last_order_update")
                : std::optional<finance::OrderUpdate>{std::nullopt}};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderProxyMonitorRequest& data) {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["symbol"] = ConvertToJson(data.symbol);

  if (data.last_order_update.has_value()) {
    json["last_order_update"] = ConvertToJson(*data.last_order_update);
  }

  return json;
}

template <>
std::optional<std::vector<finance::OrderProxyMonitorRequest>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<finance::OrderProxyMonitorRequest>>(
      json);
}

web::json::value ConvertToJson(
    const std::vector<finance::OrderProxyMonitorRequest>& data) {
  return ConvertToJsonArray(data);
}

template <>
std::optional<finance::OrderMonitorOrderUpdate> ParseFromJson(
    const web::json::value& json) {
  try {
    return finance::OrderMonitorOrderUpdate{
        .order_uuid = GetStringPropertyAsUuid(json, "order_uuid"),
        .order_update = GetObjectPropertyAsObject<finance::OrderUpdate>(
            json, "order_update")};
  } catch (const std::exception& exception) {
    spdlog::error("Parse from JSON: {}", exception.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderMonitorOrderUpdate& data) {
  auto json = web::json::value{};
  json["order_uuid"] = ConvertToJson(data.order_uuid);
  json["order_update"] = ConvertToJson(data.order_update);
  return json;
}

template <>
std::optional<std::vector<finance::OrderMonitorOrderUpdate>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<std::vector<finance::OrderMonitorOrderUpdate>>(
      json);
}

web::json::value ConvertToJson(
    const std::vector<finance::OrderMonitorOrderUpdate>& data) {
  return ConvertToJsonArray(data);
}

// template <>
// std::optional<finance::OrderMonitorOrderState> ParseFromJson(
//     const web::json::value& json) {
//   return std::nullopt;
// }

// web::json::value ConvertToJson(const finance::OrderMonitorOrderState& data)
// {
//   return {};
// }
}  // namespace stonks::json