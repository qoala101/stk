#ifndef STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_CLIENT_H_
#define STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_CLIENT_H_

#include <cppcoro/task.hpp>

#include "core_i_symbol_prices_streams_controller.h"
#include "core_types.h"
#include "service_client.h"
#include "service_spsc_traits.h"  // IWYU pragma: keep

namespace stonks::service {
/**
 * @copydoc core::ISymbolPricesStreamsController
 */
template <>
class ServiceClient<core::ISymbolPricesStreamsController>
    : public ServiceClientBase<core::ISymbolPricesStreamsController> {
 public:
  using ServiceClientBase::ServiceClientBase;

  /**
   * @copydoc core::ISymbolPricesStreamsController::GetSymbolForStream
   */
  auto GetSymbolForStream [[nodiscard]] () const
      -> cppcoro::task<core::Symbol> override;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOL_PRICES_STREAMS_CONTROLLER_SERVICE_SPSC_CLIENT_H_
