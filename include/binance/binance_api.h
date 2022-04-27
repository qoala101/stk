#ifndef STONKS_BINANCE_BINANCE_API_H_
#define STONKS_BINANCE_BINANCE_API_H_

#include <chrono>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "binance_types.h"
#include "utils.h"

namespace stonks::binance {
std::optional<std::vector<std::string>> GetSymbols();

std::optional<std::string> GetBalances();

std::variant<std::monostate, PlaceOrderAcknowledgement, ApiError> PlaceOrder(
    std::string_view symbol, OrderSide side, OrderType type,
    std::optional<OrderTimeInForce> time_in_force = std::nullopt,
    std::optional<double> quantity = std::nullopt,
    std::optional<double> quote_order_quantity = std::nullopt,
    std::optional<double> price = std::nullopt,
    std::optional<std::string_view> new_client_order_id = std::nullopt,
    std::optional<double> stop_price = std::nullopt,
    std::optional<int64_t> trailing_delta = std::nullopt,
    std::optional<double> iceberg_quantity = std::nullopt,
    std::optional<NewOrderResponseType> new_order_response_type =
        NewOrderResponseType::kAcknowledgement,
    std::optional<int64_t> receiving_window = std::nullopt,
    std::chrono::milliseconds timestamp = utils::GetUnixTime());

std::optional<std::vector<OrderInfo>> GetAllOrders(
    std::string_view symbol, std::optional<int64_t> order_id = std::nullopt,
    std::optional<std::chrono::milliseconds> start_time = std::nullopt,
    std::optional<std::chrono::milliseconds> end_time = std::nullopt,
    std::optional<int> limit = std::nullopt,
    std::optional<int64_t> receiving_window = std::nullopt,
    std::chrono::milliseconds timestamp = utils::GetUnixTime());

std::optional<OrderInfo> QueryOrder(
    std::string_view symbol, std::optional<int64_t> order_id = std::nullopt,
    std::optional<std::string> original_client_order_id = std::nullopt,
    std::optional<int64_t> receiving_window = std::nullopt,
    std::chrono::milliseconds timestamp = utils::GetUnixTime());

std::optional<std::vector<Kline>> GetKlines(
    std::string_view symbol, CandleInterval interval,
    std::optional<std::chrono::milliseconds> start_time = std::nullopt,
    std::optional<std::chrono::milliseconds> end_time = std::nullopt,
    std::optional<int> limit = std::nullopt);

std::optional<AverageSymbolPrice> GetAverageSymbolPrice(
    std::string_view symbol);

std::optional<SymbolPrice> GetSymbolPrice(
    std::optional<std::string_view> symbol = std::nullopt);
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_BINANCE_API_H_
