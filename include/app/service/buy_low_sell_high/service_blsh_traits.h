#ifndef STONKS_APP_SERVICE_BUY_LOW_SELL_HIGH_SERVICE_BLSH_TRAITS_H_
#define STONKS_APP_SERVICE_BUY_LOW_SELL_HIGH_SERVICE_BLSH_TRAITS_H_

#include "core_buy_low_sell_high.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&core::BuyLowSellHigh::CalculateNextOperations> {
  static constexpr auto kMethod = network::Method::kGet;
  static constexpr auto kParams =
      ParamList("symbol", "profit", "last_operation", "start_time", "end_time");
};

template <>
struct ClientServerTypeTraits<core::BuyLowSellHigh> {
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&core::BuyLowSellHigh::CalculateNextOperations);
};
}  // namespace stonks::networkx

#endif  // STONKS_APP_SERVICE_BUY_LOW_SELL_HIGH_SERVICE_BLSH_TRAITS_H_
