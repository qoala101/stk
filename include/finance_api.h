#ifndef STONKS_FINANCE_API_H_
#define STONKS_FINANCE_API_H_

#include <chrono>
#include <optional>
#include <string_view>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace stonks::finance {
/**
 * @remark If end time is specified, call would block untill all the data up
 * till the end time is received.
 * @remark If end time is not specified, call would block untill all the data up
 * till the current moment is recived.
 * @remark Result would only contain candlesticks whose both open and close time
 * entirely fit in the period.
 * @remark If data for particular candlestick is missing on server, candlestick
 * would have only open and close time.
 * @return Nullopt if server error occured at any time. Empty vector if none of
 * the candlesticks fully match the period.
 */
std::optional<std::vector<Candlestick>> GetCandlesticks(
    const Symbol &symbol, Interval interval,
    std::chrono::milliseconds start_time,
    std::chrono::milliseconds end_time = utils::GetUnixTime());

std::chrono::milliseconds CeilStartTimeToInterval(
    std::chrono::milliseconds time, Interval interval);

std::chrono::milliseconds FloorEndTimeToInterval(std::chrono::milliseconds time,
                                                 Interval interval);

int CanculateNumIntervalsInPeriod(std::chrono::milliseconds start_time,
                                  std::chrono::milliseconds end_time,
                                  Interval interval);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_API_H_
