#ifndef STONKS_BINANCE_API_H_
#define STONKS_BINANCE_API_H_

#include <optional>
#include <string>
#include <vector>

namespace stonks::binance {
using Symbol = std::string;

std::optional<std::vector<Symbol>> GetSymbols();

using Balance = std::string;

std::optional<std::vector<Balance>> GetBalances();
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_API_H_
