#ifndef VH_BINANCE_API_TRAITS_H_
#define VH_BINANCE_API_TRAITS_H_

#include "binance_api.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace vh::networkx {
template <>
struct EndpointFunctionTraits<&binance::BinanceApi::exchangeInfo> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<binance::BinanceApi> {
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&binance::BinanceApi::exchangeInfo);
};
}  // namespace vh::networkx

#endif  // VH_BINANCE_API_TRAITS_H_
