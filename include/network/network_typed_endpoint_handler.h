#ifndef STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_HANDLER_H_
#define STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_HANDLER_H_

#include <type_traits>

#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Decorator of request handler which adds validation of request
 * and response types.
 */
class TypedEndpointHandler {
 public:
  /**
   * @param endpoint_types Request and response would be validated against
   * those.
   */
  explicit TypedEndpointHandler(EndpointTypes endpoint_types,
                                RestRequestHandler handler);

  /**
   * @brief Validates request prior forwarding it to the handler and response
   * after receiving it from handler.
   */
  auto operator()(RestRequest request) const -> RestResponse;

 private:
  EndpointTypes endpoint_types_{};
  RestRequestHandler handler_{};
};

static_assert(std::is_convertible_v<TypedEndpointHandler, RestRequestHandler>);
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_HANDLER_H_
