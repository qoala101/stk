#ifndef STONKS_BINANCE_API_H_
#define STONKS_BINANCE_API_H_

#include <vector>
#include <string>
#include <optional>

namespace stonks::binance
{
  using Symbol = std::string;

  std::optional<std::vector<Symbol>> GetSymbols();
}

#endif // STONKS_BINANCE_API_H_
