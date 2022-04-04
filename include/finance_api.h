#ifndef STONKS_FINANCE_API_H_
#define STONKS_FINANCE_API_H_

#include <optional>
#include <string_view>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"

namespace stonks::finance {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval, int64_t history_start_time,
    int64_t history_end_time);
}

#endif  // STONKS_FINANCE_API_H_
