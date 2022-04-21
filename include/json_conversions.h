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
std::optional<finance::Symbol> ParseFromJson(const web::json::value &json);
web::json::value ConvertToJson(const finance::Symbol &data);

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
}  // namespace stonks

#endif  // STONKS_JSON_CONVERSIONS_H_
