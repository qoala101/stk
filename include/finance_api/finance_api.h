#ifndef STONKS_FINANCE_API_FINANCE_API_H_
#define STONKS_FINANCE_API_FINANCE_API_H_

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <optional>
#include <string_view>
#include <vector>

#include "finance_enums.h"
#include "finance_types.h"
#include "utils.h"

namespace stonks::finance {
/**
 * @remark If end time is specified, call would block until all the data up
 * till the end time is received.
 * @remark If end time is not specified, call would block until all the data up
 * till the current moment is received.
 * @remark Result would only contain candles whose both open and close time
 * entirely fit in the period.
 * @remark If data for particular candle is missing on server, candle
 * would have only open and close time.
 * @return Nullopt if server error ocurred at any time. Empty vector if none of
 * the candles fully match the period.
 */
std::optional<std::vector<Candle>> GetCandles(
    const Symbol &symbol, Interval interval,
    std::chrono::milliseconds start_time,
    std::chrono::milliseconds end_time = utils::GetUnixTime());

std::chrono::milliseconds CeilStartTimeToInterval(
    std::chrono::milliseconds time, Interval interval);

std::chrono::milliseconds FloorEndTimeToInterval(std::chrono::milliseconds time,
                                                 Interval interval);

int CalculateNumIntervalsInPeriod(std::chrono::milliseconds start_time,
                                  std::chrono::milliseconds end_time,
                                  Interval interval);

std::optional<OrderError> PlaceOrder(
    const OrderProxyOrderRequest &order_request);

std::optional<OrderInfo> GetOrderInfo(const Symbol &symbol,
                                      boost::uuids::uuid uuid);

/**
 * @brief Check whether order should not be monitored further.
 */
bool IsOrderStatusFinal(OrderStatus order_status);

std::optional<double> GetAverageSymbolPrice(const Symbol &symbol);

std::optional<double> GetSymbolPrice(const Symbol &symbol);

std::optional<std::vector<SymbolPrice>> GetAllSymbolsPrices();

std::optional<std::vector<Symbol>> GetAllSymbols();

auto GetSymbolsInfo() -> std::optional<std::vector<SymbolInfo>>;
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_API_FINANCE_API_H_
