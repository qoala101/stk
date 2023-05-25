/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_BINANCE_TYPES_H_
#define VH_BINANCE_TYPES_H_

#include <string>

#include "cpp_typed_struct.h"

namespace vh::binance {
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
}  // namespace vh::binance

#endif  // VH_BINANCE_TYPES_H_
