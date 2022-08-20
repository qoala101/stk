#ifndef STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
#define STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_

#include <map>
#include <memory>

#include "network_i_rest_request_handler.h"
#include "network_types.h"
#include "not_null.hpp"

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
      std::map<Endpoint, cpp::not_null<std::unique_ptr<IRestRequestHandler>>>
          endpoint_handlers);

  /**
   * @brief Forwards the request to the appropriate handler.
   */
  [[nodiscard]] auto HandleRequestAndGiveResponse(RestRequest request) const
      -> RestResponse override;

 private:
  std::map<Endpoint, cpp::not_null<std::unique_ptr<IRestRequestHandler>>>
      endpoint_handlers_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_ENDPOINT_REQUEST_DISPATCHER_H_
