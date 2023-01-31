#ifndef STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BLSH_TYPES_H_
#define STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BLSH_TYPES_H_

#include <absl/time/time.h>

namespace stonks::core::blsh {
enum class OperationType { kBuy, kSell };

struct Operation {
  absl::Time time{};
  OperationType type{};

  /**
   * @brief Input: price used to buy BTC.
   */
  double btc_buy_price{};

  /**
   * @brief Output: price we wait to sell BTC.
   */
  double btc_sell_price_waiting_for{};

  /**
   * @brief
   * Input: Amount of BTC we had before and which we would also sell along with
   * the one we would buy. Output: Amount of BTC we would have on balance after
   * we sell all our BTC.
   */
  double btc_balance{};

  /**
   * @brief
   * Input: USD amount we want to spend to buy BTC.
   * Output: Amount of USD we would have on balance after we sell all our BTC.
   */
  double usd_balance{};
};
}  // namespace stonks::core::blsh

#endif  // STONKS_APP_CORE_BUY_LOW_SELL_HIGH_CORE_BLSH_TYPES_H_
