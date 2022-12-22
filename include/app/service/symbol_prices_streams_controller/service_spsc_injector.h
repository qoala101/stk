#ifndef STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_INJECTOR_H_
#define STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_INJECTOR_H_

#include <fmt/format.h>

#include "core_i_symbol_prices_streams_controller.h"
#include "di_bind_interface_to_implementation.h"
#include "di_bind_type_to_value.h"
#include "network_types.h"
#include "service_spsc_options.h"
#include "service_symbol_prices_streams_controller.h"

namespace stonks::service::spsc {
inline auto CreateInjector [[nodiscard]] (const Options &options) {
  return di::MakeInjector(
      stonks::di::BindTypeToValue<
          stonks::networkx::Uri<core::ISymbolPricesStreamsController>>(
          stonks::networkx::Uri<core::ISymbolPricesStreamsController>{
              fmt::format("http://{}:{}",
                          *options.symbol_prices_streams_controller_host,
                          *options.symbol_prices_streams_controller_port)}),
      stonks::di::BindInterfaceToImplementation<
          stonks::core::ISymbolPricesStreamsController,
          stonks::service::SymbolPricesStreamsController>());
}
}  // namespace stonks::service::spsc

#endif  // STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_INJECTOR_H_
