#ifndef STONKS_BINANCE_BINANCE_TYPES_H_
#define STONKS_BINANCE_BINANCE_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace stonks::binance {
struct SymbolExchangeInfo {
  std::string symbol{};
  std::string base_asset{};
  std::string quote_asset{};
  std::string min_quantity{};
  std::string step_size{};
  std::string min_notional{};
  std::string tick_size{};
};

struct ExchangeInfo : public cpp::TypedStruct<std::vector<SymbolExchangeInfo>> {
};

struct BookTick {
  std::string best_bid_price{};
  std::string best_ask_price{};
};
}  // namespace stonks::binance

#endif  // STONKS_BINANCE_BINANCE_TYPES_H_
