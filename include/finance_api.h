#ifndef STONKS_FINANCE_API_H_
#define STONKS_FINANCE_API_H_

#include <chrono>
#include <optional>
#include <string_view>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"

namespace stonks::finance {
/**
 * @remark If end time is specified, call would block untill all the data up
 * till the end time is received.
 * @return Nullopt if server error occured at any time. Empty vector if
 * requested data is missing on server.
 */
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval,
    std::chrono::milliseconds start_time,
    std::optional<std::chrono::milliseconds> end_time = std::nullopt);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_API_H_
