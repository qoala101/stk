#ifndef STONKS_BINANCE_API_H_
#define STONKS_BINANCE_API_H_

#include <optional>
#include <string>
#include <vector>

#include "binance_types/types.h"

namespace stonks::binance {
using Symbol = std::string;

std::optional<std::vector<Symbol>> GetSymbols();

using Balances = std::string;

std::optional<Balances> GetBalances();

std::optional<PlaceOrderResult> PlaceOrder(
    const Symbol &symbol, Side side, Type type, std::optional<double> quantity,
    std::optional<double> price,
    TimeInForce time_in_force = TimeInForce::kGoodTillCanceled);
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_API_H_
