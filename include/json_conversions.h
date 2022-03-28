#ifndef STONKS_BINANCE_TYPES_JSON_CONVERSIONS_H_
#define STONKS_BINANCE_TYPES_JSON_CONVERSIONS_H_

#include <cpprest/json.h>

#include "binance_types/types.h"

namespace stonks {
template <typename T>
std::optional<T> ParseFromJson(const web::json::value &json) = delete;

template <>
std::optional<binance::PlaceOrderResult> ParseFromJson(
    const web::json::value &json);
}  // namespace stonks

#endif  // STONKS_BINANCE_TYPES_JSON_CONVERSIONS_H_
