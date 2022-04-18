#ifndef STONKS_BINANCE_BINANCE_API_H_
#define STONKS_BINANCE_BINANCE_API_H_

#include <chrono>
#include <optional>
#include <string>
#include <vector>

#include "binance_types.h"

namespace stonks::binance {
std::optional<std::vector<std::string>> GetSymbols();

std::optional<std::string> GetBalances();

std::optional<PlaceOrderResult> PlaceOrder(
    std::string_view symbol, Side side, Type type,
    std::optional<double> quantity = std::nullopt,
    std::optional<double> price = std::nullopt,
    TimeInForce time_in_force = TimeInForce::kGoodTillCanceled);

std::optional<std::vector<Kline>> GetKlines(
    std::string_view symbol, CandleInterval interval,
    std::optional<std::chrono::milliseconds> start_time = std::nullopt,
    std::optional<std::chrono::milliseconds> end_time = std::nullopt,
    std::optional<int> limit = std::nullopt);
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_BINANCE_API_H_
