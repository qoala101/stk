#ifndef STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_TRAITS_H_
#define STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_TRAITS_H_

#include "core_i_symbol_prices_streams_controller.h"
#include "networkx_common.h"
#include "networkx_types.h"

namespace stonks::networkx {
template <>
struct EndpointFunctionTraits<
    &core::ISymbolPricesStreamsController::GetSymbolForStream> {
  static constexpr auto kMethod = network::Method::kGet;
};

template <>
struct ClientServerTypeTraits<core::ISymbolPricesStreamsController> {
  static constexpr auto kEndpointFunctions = EndpointFunctionList(
      &core::ISymbolPricesStreamsController::GetSymbolForStream);
};
}  // namespace stonks::networkx

#endif  // STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_TRAITS_H_
