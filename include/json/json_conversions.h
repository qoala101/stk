#ifndef STONKS_JSON_JSON_CONVERSIONS_H_
#define STONKS_JSON_JSON_CONVERSIONS_H_

#include <cpprest/json.h>

#include <any>
#include <chrono>
#include <cstdint>
#include <magic_enum.hpp>
#include <variant>
#include <vector>

#include "binance_types.h"
#include "concepts.h"
#include "finance_types.h"

namespace stonks::json {
template <Enumeration T>
web::json::value ConvertToJson(T data) {
  return web::json::value::string(std::string{magic_enum::enum_name(data)});
}

template <Number T>
web::json::value ConvertToJson(T data) {
  return web::json::value::number(data);
}

web::json::value ConvertToJson(const std::string &data);
web::json::value ConvertToJson(std::string_view data);
web::json::value ConvertToJson(const char *data);
web::json::value ConvertToJson(std::chrono::milliseconds data);
web::json::value ConvertToJson(boost::uuids::uuid data);

template <typename T>
std::optional<T> ParseFromJson(const web::json::value &json) = delete;

template <>
auto ParseFromJson(const web::json::value &json) -> std::optional<std::string>;
template <>
auto ParseFromJson(const web::json::value &json) -> std::optional<int>;
template <>
auto ParseFromJson(const web::json::value &json) -> std::optional<int64_t>;
template <>
auto ParseFromJson(const web::json::value &json) -> std::optional<double>;
template <>
auto ParseFromJson(const web::json::value &json)
    -> std::optional<std::chrono::milliseconds>;

template <>
std::optional<binance::PlaceOrderAcknowledgement> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::OrderInfo> ParseFromJson(const web::json::value &json);

template <>
std::optional<binance::ApiError> ParseFromJson(const web::json::value &json);

template <>
std::optional<std::vector<binance::OrderInfo>> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::Kline> ParseFromJson(const web::json::value &json);

template <>
std::optional<std::vector<binance::Kline>> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::AverageSymbolPrice> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::SymbolPrice> ParseFromJson(const web::json::value &json);

template <>
std::optional<std::vector<binance::SymbolPrice>> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::SymbolExchangeInfo> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<std::vector<binance::SymbolExchangeInfo>> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::ExchangeInfo> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::SymbolBookTicker> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<finance::Symbol> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::Symbol &data);

template <>
std::optional<finance::TimeDouble> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::TimeDouble &data);

template <>
std::optional<std::vector<finance::TimeDouble>> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const std::vector<finance::TimeDouble> &data);

template <>
std::optional<finance::SymbolPrices> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::SymbolPrices &data);

template <>
std::optional<std::vector<finance::SymbolPrices>> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const std::vector<finance::SymbolPrices> &data);

template <>
auto ParseFromJson(const web::json::value &json)
    -> std::optional<finance::SymbolPriceTick>;
auto ConvertToJson(const finance::SymbolPriceTick &data) -> web::json::value;

template <>
auto ParseFromJson(const web::json::value &json)
    -> std::optional<std::vector<finance::SymbolPriceTick>>;
auto ConvertToJson(const std::vector<finance::SymbolPriceTick> &data)
    -> web::json::value;

template <>
std::optional<finance::StrategyData> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::StrategyData &data);

template <>
std::optional<finance::OrderType> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderType &data);

template <>
std::optional<finance::OrderUpdate> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderUpdate &data);

template <>
std::optional<finance::OrderProxyOrderUpdate> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderProxyOrderUpdate &data);

template <>
std::optional<std::vector<finance::OrderProxyOrderUpdate>> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(
    const std::vector<finance::OrderProxyOrderUpdate> &data);

template <>
std::optional<finance::Amount> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::Amount &data);

template <>
std::optional<finance::Order> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::Order &data);

template <>
std::optional<finance::StrategyOrderRequest> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::StrategyOrderRequest &data);

template <>
std::optional<finance::OrderProxyOrderRequest> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderProxyOrderRequest &data);

template <>
std::optional<finance::StrategySubscribeToOrderUpdatesRequest> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(
    const finance::StrategySubscribeToOrderUpdatesRequest &data);

template <>
auto ParseFromJson(const web::json::value &json)
    -> std::optional<finance::PriceTicksServiceSubscribeRequest>;
auto ConvertToJson(const finance::PriceTicksServiceSubscribeRequest &data)
    -> web::json::value;

template <>
std::optional<finance::OrderProxyMonitorRequest> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderProxyMonitorRequest &data);

template <>
std::optional<std::vector<finance::OrderProxyMonitorRequest>> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(
    const std::vector<finance::OrderProxyMonitorRequest> &data);

template <>
std::optional<finance::OrderMonitorOrderUpdate> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderMonitorOrderUpdate &data);

template <>
std::optional<std::vector<finance::OrderMonitorOrderUpdate>> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(
    const std::vector<finance::OrderMonitorOrderUpdate> &data);

template <>
std::optional<finance::OrderMonitorOrderState> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderMonitorOrderState &data);

template <typename T>
class JsonConverter {
 public:
  using Type = T;

  [[nodiscard]] auto ParseFromJson(const web::json::value &json) const
      -> std::optional<T> {
    return ::stonks::json::ParseFromJson<T>(json);
  }

  [[nodiscard]] auto ConvertToJson(const T &data) const -> web::json::value {
    return ::stonks::json::ConvertToJson(data);
  }

  /**
   * @throws If any stores wrong type.
   */
  [[nodiscard]] auto any_ConvertToJson(const std::any &data) const
      -> web::json::value {
    return ::stonks::json::ConvertToJson(std::any_cast<T>(data));
  }

  /**
   * @throws If any stores wrong type.
   */
  [[nodiscard]] auto any_ParseFromJson(const web::json::value &json) const
      -> std::any {
    auto object = ::stonks::json::ParseFromJson<T>(json);

    if (object.has_value()) {
      return std::move(*object);
    }

    return {};
  }
};

using JsonConverterVariant =
    std::variant<JsonConverter<std::string>, JsonConverter<int>,
                 JsonConverter<int64_t>, JsonConverter<double>,
                 JsonConverter<std::chrono::milliseconds>,
                 JsonConverter<finance::Symbol>>;

struct Any {
  std::any value{};
  JsonConverterVariant converter{};
} __attribute__((aligned(32)));  // NOLINT(*-magic-numbers)

enum class OptionalFlag { kOptional, kMandatory };

struct AnyDesc {
  json::JsonConverterVariant converter{};
  OptionalFlag optional{};
} __attribute__((aligned(8)));  // NOLINT(*-magic-numbers)
}  // namespace stonks::json

#endif  // STONKS_JSON_JSON_CONVERSIONS_H_
