#ifndef STONKS_BINANCE_BINANCE_API_TRAITS_H_
#define STONKS_BINANCE_BINANCE_API_TRAITS_H_

#include "binance_api.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&binance::BinanceApi::exchangeInfo> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<binance::BinanceApi> {
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&binance::BinanceApi::exchangeInfo);
};
}  // namespace stonks::networkx

#endif  // STONKS_BINANCE_BINANCE_API_TRAITS_H_
