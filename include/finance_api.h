#ifndef STONKS_FINANCE_API_H_
#define STONKS_FINANCE_API_H_

#include <optional>
#include <string_view>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"

namespace stonks::finance {
/**
 * @remark If requested amount is greater than 10000, nullopt is returned.
 * @remark If end time is same as start time, single candlestick is returned.
 */
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval, int64_t history_start_time_ms,
    int64_t history_end_time_ms);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_API_H_
