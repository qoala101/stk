#include "json_conversions.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <magic_enum.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_enum_conversions.h"

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

std::chrono::milliseconds GetInt64PropertyAsMilliseconds(
    const web::json::value& json, const std::string_view property_name) {
  return std::chrono::milliseconds{
      json.at(std::string{property_name}).as_number().to_int64()};
}

std::chrono::milliseconds GetInt64ElementAsMilliseconds(
    const web::json::value& json, const int index) {
  return std::chrono::milliseconds{json.at(index).as_number().to_int64()};
}

template <typename E>
E GetStringPropertyAsEnum(const web::json::value& json,
                          const std::string_view property_name) {
  return magic_enum::enum_cast<E>(
             json.at(std::string{property_name}).as_string())
      .value_or(E::kInvalid);
}

template <typename E>
E GetStringElementAsEnum(const web::json::value& json, const int index) {
  return magic_enum::enum_cast<E>(json.at(index).as_string())
      .value_or(E::kInvalid);
}
}  // namespace

namespace stonks {
template <>
std::optional<binance::PlaceOrderResult> ParseFromJson(
    const web::json::value& json) {
  try {
    return binance::PlaceOrderResult{
        .client_order_id = GetStringPropertyAsString(json, "clientOrderId"),
        .cummulative_quote_quantity =
            GetStringPropertyAsDouble(json, "cummulativeQuoteQty"),
        .executed_quantity = GetStringPropertyAsDouble(json, "executedQty"),
        // .fills
        .order_id = GetInt64PropertyAsInt64(json, "orderId"),
        .order_list_id = GetInt64PropertyAsInt64(json, "orderListId"),
        .original_quantity = GetStringPropertyAsDouble(json, "origQty"),
        .price = GetStringPropertyAsDouble(json, "price"),
        .side = GetStringPropertyAsEnum<binance::Side>(json, "side"),
        .status = GetStringPropertyAsEnum<binance::Status>(json, "status"),
        .symbol = GetStringPropertyAsString(json, "symbol"),
        .time_in_force =
            GetStringPropertyAsEnum<binance::TimeInForce>(json, "timeInForce"),
        .transaction_time = GetInt64PropertyAsMilliseconds(json, "transactTime"),
        .type = GetStringPropertyAsEnum<binance::Type>(json, "type")};
  } catch (const std::exception& exeption) {
    spdlog::error("Parse from JSON: {}", exeption.what());
  } catch (...) {
    spdlog::error("Parse from JSON: {}", "Unknown error");
  }

  return std::nullopt;
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
  if (!json.is_array()) {
    spdlog::error("Parse from JSON: {}", "not array");
    return std::nullopt;
  }

  const auto& klines_array = json.as_array();

  const auto parse_kline = [](const web::json::value& value) {
    return ParseFromJson<binance::Kline>(value);
  };
  auto kline_succesfully_parsed =
      [](const std::optional<binance::Kline>& kline) {
        return kline.has_value();
      };
  const auto get_kline = [](const std::optional<binance::Kline>& kline) {
    return std::move(*kline);
  };

  const auto parsed_klines =
      klines_array | ranges::views::transform(parse_kline) |
      ranges::views::take_while(kline_succesfully_parsed) |
      ranges::views::transform(get_kline) | ranges::to_vector;

  if (parsed_klines.size() < klines_array.size()) {
    spdlog::error("Parse from JSON: {}", "cannot parse all klines");
    return std::nullopt;
  }

  return parsed_klines;
}
}  // namespace stonks