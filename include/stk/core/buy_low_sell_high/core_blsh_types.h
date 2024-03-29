/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_CORE_BLSH_TYPES_H_
#define VH_STK_CORE_BLSH_TYPES_H_

#include <absl/time/time.h>

namespace vh::stk::core::blsh {
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
}  // namespace vh::stk::core::blsh

#endif  // VH_STK_CORE_BLSH_TYPES_H_
