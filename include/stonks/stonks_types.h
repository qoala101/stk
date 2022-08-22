#ifndef STONKS_STONKS_STONKS_TYPES_H_
#define STONKS_STONKS_STONKS_TYPES_H_

#include <chrono>
#include <string>

#include "ccutils_optional.h"

namespace stonks {
using SymbolName = std::string;

struct SymbolInfo {
  SymbolName symbol;
  std::string base_asset{};
  std::string quote_asset{};
  double min_base_amount{};
  double min_quote_amount{};
  double base_step{};
  double quote_step{};

 private:
  [[nodiscard]] friend auto operator==(const SymbolInfo &, const SymbolInfo &)
      -> bool = default;
  [[nodiscard]] friend auto operator<=>(const SymbolInfo &, const SymbolInfo &)
      -> std::partial_ordering = default;
};

struct SymbolPriceTick {
  SymbolName symbol{};
  std::chrono::milliseconds time{};
  double buy_price{};
  double sell_price{};

 private:
  [[nodiscard]] friend auto operator==(const SymbolPriceTick &left,
                                       const SymbolPriceTick &right)
      -> bool = default;
  [[nodiscard]] friend auto operator<=>(const SymbolPriceTick &left,
                                        const SymbolPriceTick &right)
      -> std::partial_ordering = default;
};

struct Period {
  ccutils::Opt<std::chrono::milliseconds> start_time{};
  ccutils::Opt<std::chrono::milliseconds> end_time{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_TYPES_H_
