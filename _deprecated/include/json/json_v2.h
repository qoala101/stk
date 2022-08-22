#ifndef STONKS_JSON_JSON_V2_H_
#define STONKS_JSON_JSON_V2_H_

#include <bits/exception.h>
#include <cpprest/json.h>

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <cstdint>
#include <magic_enum.hpp>
#include <optional>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "binance_types.h"
#include "concepts.h"  // IWYU pragma: keep
#include "finance_types.h"
#include "ngrok_types.h"

namespace stonks::json {
void ParseFromJson(const web::json::value &json, bool &data);
void ConvertToJson(const bool &data, web::json::value &json);
void ParseFromJson(const web::json::value &json, int &);
void ParseFromJson(const web::json::value &json, int64_t &);
void ParseFromJson(const web::json::value &json, double &);

template <Number T>
void ConvertToJson(const T &data, web::json::value &json) {
  json = web::json::value::number(data);
}

template <Enumeration T>
void ParseFromJson(const web::json::value &json, T &data) {
  data = magic_enum::enum_cast<T>(json.as_string()).value_or(T::kInvalid);
}

template <Enumeration T>
void ConvertToJson(const T &data, web::json::value &json) {
  json = web::json::value::string(std::string{magic_enum::enum_name(data)});
}

void ParseFromJson(const web::json::value &json, std::string &data);
void ConvertToJson(const std::string &data, web::json::value &json);
void ConvertToJson(std::string_view data, web::json::value &json);
void ConvertToJson(const char *data, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   std::chrono::milliseconds &data);
void ConvertToJson(std::chrono::milliseconds data, web::json::value &json);

void ParseFromJson(const web::json::value &json, boost::uuids::uuid &data);
void ConvertToJson(boost::uuids::uuid data, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   binance::PlaceOrderAcknowledgement &data);

void ParseFromJson(const web::json::value &json, binance::ApiError &data);

void ParseFromJson(const web::json::value &json, binance::OrderInfo &data);

void ParseFromJson(const web::json::value &json, binance::Kline &data);

void ParseFromJson(const web::json::value &json,
                   binance::AverageSymbolPrice &data);

void ParseFromJson(const web::json::value &json, binance::SymbolPrice &data);

void ParseFromJson(const web::json::value &json,
                   binance::SymbolExchangeInfo &data);

void ParseFromJson(const web::json::value &json, binance::ExchangeInfo &data);
void ConvertToJson(const binance::ExchangeInfo &, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   binance::SymbolBookTicker &data);

void ParseFromJson(const web::json::value &json, finance::Symbol &data);
void ConvertToJson(const finance::Symbol &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::TimeDouble &data);
void ConvertToJson(const finance::TimeDouble &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::SymbolPrices &data);
void ConvertToJson(const finance::SymbolPrices &data, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::SymbolPriceTick &data);
void ConvertToJson(const finance::SymbolPriceTick &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::StrategyData &data);
void ConvertToJson(const finance::StrategyData &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::OrderType &data);
void ConvertToJson(const finance::OrderType &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::OrderUpdate &data);
void ConvertToJson(const finance::OrderUpdate &data, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::OrderProxyOrderUpdate &data);
void ConvertToJson(const finance::OrderProxyOrderUpdate &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::Amount &data);
void ConvertToJson(const finance::Amount &data, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::StrategyOrderRequest &data);
void ConvertToJson(const finance::StrategyOrderRequest &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::OrderProxyOrderRequest &data);
void ConvertToJson(const finance::OrderProxyOrderRequest &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::StrategySubscribeToOrderUpdatesRequest &data);
void ConvertToJson(const finance::StrategySubscribeToOrderUpdatesRequest &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::PriceTicksServiceSubscribeRequest &data);
void ConvertToJson(const finance::PriceTicksServiceSubscribeRequest &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::OrderProxyMonitorRequest &data);
void ConvertToJson(const finance::OrderProxyMonitorRequest &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::OrderMonitorOrderUpdate &data);
void ConvertToJson(const finance::OrderMonitorOrderUpdate &data,
                   web::json::value &json);

void ParseFromJson(const web::json::value &json, std::runtime_error &data);
void ConvertToJson(const std::runtime_error &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::Period &data);
void ConvertToJson(const finance::Period &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, finance::SymbolInfo &data);
void ConvertToJson(const finance::SymbolInfo &data, web::json::value &json);

void ParseFromJson(const web::json::value &json, ngrok::Tunnel &data);
void ConvertToJson(const ngrok::Tunnel &data, web::json::value &json);

void ParseFromJson(const web::json::value &json,
                   finance::SymbolBalanceTick &data);
void ConvertToJson(const finance::SymbolBalanceTick &data,
                   web::json::value &json);

template <Vector T>
void ParseFromJson(const web::json::value &json, T &data) {
  if (!json.is_array()) {
    throw std::runtime_error{"Cannot parse array"};
  }

  const auto parse_item = [](const web::json::value &value) {
    auto data = typename T::value_type{};
    ParseFromJson(value, data);
    return data;
  };

  data = json.as_array() | ranges::views::transform(parse_item) |
         ranges::to_vector;
}

template <typename T>
void ConvertToJson(const std::vector<T> &data, web::json::value &json) {
  const auto convert_item = [](const T &item) {
    auto json = web::json::value{};
    ConvertToJson(item, json);
    return json;
  };

  auto json_items =
      data | ranges::views::transform(convert_item) | ranges::to_vector;

  json = web::json::value::array(std::move(json_items));
}

template <typename T>
void ConvertToJson(const std::optional<T> &data, web::json::value &json) {
  if (!data.has_value()) {
    json = web::json::value::null();
  }

  ConvertToJson(*data, json);
}

template <typename T>
void ParseFromJsonNoThrow(const web::json::value &json,
                          std::optional<T> &data) {
  try {
    ParseFromJson<T>(json, data);
    return;
  } catch (const std::exception &e) {
  }

  data = std::nullopt;
}

template <typename T>
void ConvertToJsonNoThrow(const T &data, web::json::value &json) {
  try {
    ConvertToJson(data, json);
    return;
  } catch (const std::exception &e) {
  }

  json = web::json::value::null();
}
}  // namespace stonks::json

#endif  // STONKS_JSON_JSON_V2_H_
