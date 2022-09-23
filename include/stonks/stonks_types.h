#ifndef STONKS_STONKS_STONKS_TYPES_H_
#define STONKS_STONKS_STONKS_TYPES_H_

#include <absl/time/time.h>

#include <string>

#include "cpp_optional.h"
#include "cpp_typed_struct.h"

namespace stonks {
struct SymbolName : public cpp::TypedStruct<std::string> {};

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
  absl::Time time{};
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
  cpp::Opt<absl::Time> start_time{};
  cpp::Opt<absl::Time> end_time{};
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_TYPES_H_
