#ifndef STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
#define STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_

#include <absl/container/flat_hash_map.h>
#include <cppcoro/task.hpp>
#include <map>

#include "cpp_not_null.h"
#include "network_i_rest_request_handler.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Request handler which maps endpoint requests to the distinct handlers.
 */
class EndpointRequestDispatcher : public IRestRequestHandler {
 public:
  /**
   * @param base_uri Part of URI which is subtracted from the beginning
   * of the requested URI.
   * @param endpoint_handlers Handle per each endpoint which represents
   * the remaining part of the requested URI.
   */
  explicit EndpointRequestDispatcher(
      absl::flat_hash_map<Endpoint, cpp::NnUp<IRestRequestHandler>> endpoint_handlers);

  /**
   * @brief Forwards the request to the appropriate handler.
   */
  auto HandleRequestAndGiveResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> override;

 private:
  absl::flat_hash_map<Endpoint, cpp::NnUp<IRestRequestHandler>> endpoint_handlers_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
