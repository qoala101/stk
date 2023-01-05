#ifndef STONKS_APP_SERVICE_SYMBOL_PRICE_STREAMS_SERVICE_SPS_TRAITS_H_
#define STONKS_APP_SERVICE_SYMBOL_PRICE_STREAMS_SERVICE_SPS_TRAITS_H_

#include "core_symbol_price_streams.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<&core::SymbolPriceStreams::GetStreamedSymbols> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<core::SymbolPriceStreams> {
  static constexpr auto kName = "symbol_price_streams";
  static constexpr auto kDefaultPort = "30002";
  static constexpr auto kEndpointFunctions =
      EndpointFunctionList(&core::SymbolPriceStreams::GetStreamedSymbols);
};
}  // namespace stonks::networkx

#endif  // STONKS_APP_SERVICE_SYMBOL_PRICE_STREAMS_SERVICE_SPS_TRAITS_H_
