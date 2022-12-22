#ifndef STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_H_
#define STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_H_

#include <cppcoro/task.hpp>

#include "core_i_symbol_prices_streams_controller.h"
#include "core_types.h"
#include "networkx_client.h"
#include "service_spsc_traits.h"  // IWYU pragma: keep

namespace stonks::service {
/**
 * @copydoc core::ISymbolPricesStreamsController
 */
class SymbolPricesStreamsController
    : public core::ISymbolPricesStreamsController {
 public:
  explicit SymbolPricesStreamsController(
      networkx::Client<core::ISymbolPricesStreamsController> client);

  /**
   * @copydoc core::ISymbolPricesStreamsController::GetSymbolForStream
   */
  auto GetSymbolForStream [[nodiscard]] () const
      -> cppcoro::task<core::Symbol> override;

 private:
  networkx::Client<core::ISymbolPricesStreamsController> client_;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_H_
