#ifndef STONKS_JSON_CONVERSIONS_H_
#define STONKS_JSON_CONVERSIONS_H_

#include <cpprest/json.h>

#include <vector>

#include "binance/types.h"

namespace stonks {
template <typename T>
std::optional<T> ParseFromJson(const web::json::value &json) = delete;

template <>
std::optional<binance::PlaceOrderResult> ParseFromJson(
    const web::json::value &json);

template <>
std::optional<binance::Kline> ParseFromJson(const web::json::value &json);

template <>
std::optional<std::vector<binance::Kline>> ParseFromJson(
    const web::json::value &json);
}  // namespace stonks

#endif  // STONKS_JSON_CONVERSIONS_H_
