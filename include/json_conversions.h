#ifndef STONKS_JSON_CONVERSIONS_H_
#define STONKS_JSON_CONVERSIONS_H_

#include <cpprest/json.h>

#include <vector>

#include "binance_types.h"
#include "finance_types.h"

namespace stonks {
template <typename T>
std::optional<T> ParseFromJson(const web::json::value &json) = delete;

template <>
std::optional<binance::PlaceOrderResult> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::OrderInfo> ParseFromJson(const web::json::value &json);

template <>
std::optional<std::vector<binance::OrderInfo>> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::Kline> ParseFromJson(const web::json::value &json);

template <>
std::optional<std::vector<binance::Kline>> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<finance::Symbol> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::Symbol &data);

template <>
std::optional<finance::StrategyInfo> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::StrategyInfo &data);

template <>
std::optional<finance::OrderRequest> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::OrderRequest &data);

template <>
std::optional<finance::StrategyOrderRequest> ParseFromJson(
    const web::json::value &json);
web::json::value ConvertToJson(const finance::StrategyOrderRequest &data);
}  // namespace stonks

#endif  // STONKS_JSON_CONVERSIONS_H_
