#ifndef STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
#define STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_

#include <map>
#include <type_traits>

#include "network_types.h"

namespace stonks::network {
/**
 * @brief Request handler which maps endpoint requests to the distinct handlers.
 */
class EndpointRequestDispatcher {
 public:
  /**
   * @param base_uri Part of URI which is subtracted from the beginning
   * of the requested URI.
   * @param endpoint_handlers Handle per each endpoint which represents
   * the remaining part of the requested URI.
   */
  explicit EndpointRequestDispatcher(
      std::map<Endpoint, RestRequestHandler> endpoint_handlers);

  /**
   * @brief Forwards the request to the appropriate handler.
   */
  auto operator()(RestRequest request) const -> RestResponse;

 private:
  std::map<Endpoint, RestRequestHandler> endpoint_handlers_{};
};

static_assert(
    std::is_convertible_v<EndpointRequestDispatcher, RestRequestHandler>);
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
