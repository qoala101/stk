#ifndef STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
#define STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_

#include <functional>
#include <map>
#include <type_traits>
#include <utility>

#include "network_enums.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Request handler which forwards requests to sub-endpoints
 * to the distinct handlers.
 */
class EndpointRequestDispatcher {
 public:
  /**
   * @brief Handle of sub-endpoint.
   */
  using EndpointRequestHandler = std::function<Result(Params, Body)>;

  /**
   * @param endpoint_handlers Handle per each sub-endpoint.
   * @param not_found_handler Handler for not registered sub-endpoint.
   */
  explicit EndpointRequestDispatcher(
      std::map<Endpoint, EndpointRequestHandler> endpoint_handlers,
      EndpointRequestHandler not_found_handler);

  /**
   * @brief Forwards the request to the appropriate handler.
   */
  auto operator()(const Endpoint &endpoint, RestRequestData data) const
      -> std::pair<Status, Result>;

 private:
  std::map<Endpoint, EndpointRequestHandler> endpoint_handlers_{};
  EndpointRequestHandler not_found_handler_{};
};

static_assert(
    std::is_convertible_v<EndpointRequestDispatcher, RestRequestHandler>);
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
