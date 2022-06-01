#ifndef STONKS_JSON_JSON_CONVERSIONS_H_
#define STONKS_JSON_JSON_CONVERSIONS_H_

#include <cpprest/json.h>

#include <any>
#include <chrono>
#include <magic_enum.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "binance_types.h"
#include "concepts.h"
#include "finance_types.h"
#include "ngrok_types.h"

namespace stonks::json {
template <NotVectorAndNotEnumeration T>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> T;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> bool;
[[nodiscard]] auto ConvertToJson(const bool &data) -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> int;
template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> int64_t;
template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> double;
template <Number T>
[[nodiscard]] auto ConvertToJson(T data) -> web::json::value {
  return web::json::value::number(data);
}

template <Enumeration T>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> T {
  return magic_enum::enum_cast<T>(json.as_string()).value_or(T::kInvalid);
}

template <Enumeration T>
[[nodiscard]] auto ConvertToJson(T data) -> web::json::value {
  return web::json::value::string(std::string{magic_enum::enum_name(data)});
}

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> std::string;
[[nodiscard]] auto ConvertToJson(const std::string &data) -> web::json::value;
[[nodiscard]] auto ConvertToJson(std::string_view data) -> web::json::value;
[[nodiscard]] auto ConvertToJson(const char *data) -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> std::chrono::milliseconds;
[[nodiscard]] auto ConvertToJson(std::chrono::milliseconds data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> boost::uuids::uuid;
[[nodiscard]] auto ConvertToJson(boost::uuids::uuid data) -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::PlaceOrderAcknowledgement;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::ApiError;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::OrderInfo;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::Kline;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::AverageSymbolPrice;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::SymbolPrice;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::SymbolExchangeInfo;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::ExchangeInfo;
[[nodiscard]] auto ConvertToJson(const binance::ExchangeInfo &)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> binance::SymbolBookTicker;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::Symbol;
[[nodiscard]] auto ConvertToJson(const finance::Symbol &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::TimeDouble;
[[nodiscard]] auto ConvertToJson(const finance::TimeDouble &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::SymbolPrices;
[[nodiscard]] auto ConvertToJson(const finance::SymbolPrices &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::SymbolPriceTick;
[[nodiscard]] auto ConvertToJson(const finance::SymbolPriceTick &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::StrategyData;
[[nodiscard]] auto ConvertToJson(const finance::StrategyData &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::OrderType;
[[nodiscard]] auto ConvertToJson(const finance::OrderType &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::OrderUpdate;
[[nodiscard]] auto ConvertToJson(const finance::OrderUpdate &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::OrderProxyOrderUpdate;
[[nodiscard]] auto ConvertToJson(const finance::OrderProxyOrderUpdate &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::Amount;
[[nodiscard]] auto ConvertToJson(const finance::Amount &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::StrategyOrderRequest;
[[nodiscard]] auto ConvertToJson(const finance::StrategyOrderRequest &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::OrderProxyOrderRequest;
[[nodiscard]] auto ConvertToJson(const finance::OrderProxyOrderRequest &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::StrategySubscribeToOrderUpdatesRequest;
[[nodiscard]] auto ConvertToJson(
    const finance::StrategySubscribeToOrderUpdatesRequest &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::PriceTicksServiceSubscribeRequest;
[[nodiscard]] auto ConvertToJson(
    const finance::PriceTicksServiceSubscribeRequest &data) -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::OrderProxyMonitorRequest;
[[nodiscard]] auto ConvertToJson(const finance::OrderProxyMonitorRequest &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::OrderMonitorOrderUpdate;
[[nodiscard]] auto ConvertToJson(const finance::OrderMonitorOrderUpdate &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> std::runtime_error;
[[nodiscard]] auto ConvertToJson(const std::runtime_error &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::Period;
[[nodiscard]] auto ConvertToJson(const finance::Period &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::SymbolInfo;
[[nodiscard]] auto ConvertToJson(const finance::SymbolInfo &data)
    -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> ngrok::Tunnel;
[[nodiscard]] auto ConvertToJson(const ngrok::Tunnel &data) -> web::json::value;

template <>
[[nodiscard]] auto ParseFromJson(const web::json::value &json)
    -> finance::SymbolBalanceTick;
[[nodiscard]] auto ConvertToJson(const finance::SymbolBalanceTick &data)
    -> web::json::value;

template <Vector T>
[[nodiscard]] auto ParseFromJson(const web::json::value &json) -> T {
  if (!json.is_array()) {
    throw std::runtime_error{"Cannot parse array"};
  }

  const auto parse_item = [](const web::json::value &value) {
    return ParseFromJson<typename T::value_type>(value);
  };

  return json.as_array() | ranges::views::transform(parse_item) |
         ranges::to_vector;
}

template <typename T>
[[nodiscard]] auto ConvertToJson(const std::vector<T> &data)
    -> web::json::value {
  const auto convert_item = [](const T &item) { return ConvertToJson(item); };
  auto json_items =
      data | ranges::views::transform(convert_item) | ranges::to_vector;
  return web::json::value::array(std::move(json_items));
}

template <typename T>
[[nodiscard]] auto ConvertToJson(const std::optional<T> &data)
    -> web::json::value {
  if (!data.has_value()) {
    return web::json::value::null();
  }

  return ConvertToJson(*data);
}

template <typename T>
[[nodiscard]] auto ParseFromJsonNoThrow(const web::json::value &json)
    -> std::optional<T> {
  try {
    return ParseFromJson<T>(json);
  } catch (const std::exception &e) {
  }

  return std::nullopt;
}

template <typename T>
[[nodiscard]] auto ConvertToJsonNoThrow(const T &data) -> web::json::value {
  try {
    return ConvertToJson(data);
  } catch (const std::exception &e) {
  }

  return web::json::value::null();
}
}  // namespace stonks::json

#endif  // STONKS_JSON_JSON_CONVERSIONS_H_
