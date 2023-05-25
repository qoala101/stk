/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_BLSH_TRAITS_H_
#define VH_STK_SERVICE_BLSH_TRAITS_H_

#include "core_buy_low_sell_high.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace vh::networkx {
template <>
struct EndpointFunctionTraits<
    &stk::core::BuyLowSellHigh::CalculateNextOperations> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("symbol", "profit", "last_operation", "start_time", "end_time");
};

template <>
struct ClientServerTypeTraits<stk::core::BuyLowSellHigh> {
  static constexpr auto kName = "buy_low_sell_high";
  static constexpr auto kDefaultPort = "30001";
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&stk::core::BuyLowSellHigh::CalculateNextOperations);
};
}  // namespace vh::networkx

#endif  // VH_STK_SERVICE_BLSH_TRAITS_H_
