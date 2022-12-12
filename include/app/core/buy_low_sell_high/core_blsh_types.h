#ifndef STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BLSH_TYPES_H_
#define STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BLSH_TYPES_H_

#include <absl/time/time.h>

namespace stonks::core::blsh {
enum class OperationType { kBuy, kSell };

struct Operation {
  absl::Time time{};
  double base_asset_balance{};
  double quote_asset_balance{};
  OperationType type{};
};
}  // namespace stonks::core::blsh

#endif  // STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BLSH_TYPES_H_
