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

namespace stonks {
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
std::optional<std::vector<T>> ParseFromJsonArray(const web::json::value& json) {
  if (!json.is_array()) {
    spdlog::error("Parse from JSON: {}", "not array");
    return std::nullopt;
  }

  const auto& json_array = json.as_array();

  const auto parse_item = [](const web::json::value& value) {
    return ParseFromJson<T>(value);
  };
  auto item_succesfully_parsed = [](const std::optional<T>& item) {
    return item.has_value();
  };
  const auto get_item = [](const std::optional<T>& item) {
    return std::move(*item);
  };

  const auto parsed_items = json_array | ranges::views::transform(parse_item) |
                            ranges::views::take_while(item_succesfully_parsed) |
                            ranges::views::transform(get_item) |
                            ranges::to_vector;

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
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
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
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
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
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<std::vector<binance::OrderInfo>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<binance::OrderInfo>(json);
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
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

template <>
std::optional<std::vector<binance::Kline>> ParseFromJson(
    const web::json::value& json) {
  return ParseFromJsonArray<binance::Kline>(json);
}

template <>
std::optional<finance::Symbol> ParseFromJson(const web::json::value& json) {
  try {
    return finance::Symbol{
        .base_asset = GetStringPropertyAsString(json, "base_asset"),
        .quote_asset = GetStringPropertyAsString(json, "quote_asset")};
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
}

web::json::value ConvertToJson(const finance::Symbol& data) {
  auto json = web::json::value{};
  json["base_asset"] = web::json::value::string(data.base_asset);
  json["quote_asset"] = web::json::value::string(data.quote_asset);
  return json;
}

template <>
std::optional<finance::StrategyData> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::StrategyData& data) { return {}; }

template <>
std::optional<finance::OrderType> ParseFromJson(const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderType& data) { return {}; }

template <>
std::optional<finance::OrderUpdate> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderUpdate& data) { return {}; }

template <>
std::optional<finance::OrderProxyOrderUpdate> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderProxyOrderUpdate& data) {
  return {};
}

template <>
std::optional<finance::Amount> ParseFromJson(const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::Amount& data) { return {}; }

template <>
std::optional<finance::Order> ParseFromJson(const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::Order& data) { return {}; }

template <>
std::optional<finance::StrategyOrderRequest> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::StrategyOrderRequest& data) {
  return {};
}

template <>
std::optional<finance::OrderProxyOrderRequest> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderProxyOrderRequest& data) {
  return {};
}

template <>
std::optional<finance::OrderProxyMonitorRequest> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderProxyMonitorRequest& data) {
  return {};
}

template <>
std::optional<finance::OrderMonitorOrderUpdate> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderMonitorOrderUpdate& data) {
  return {};
}

template <>
std::optional<finance::OrderMonitorOrderState> ParseFromJson(
    const web::json::value& json) {
  return std::nullopt;
}

web::json::value ConvertToJson(const finance::OrderMonitorOrderState& data) {
  return {};
}
}  // namespace stonks