#ifndef STONKS_NETWORK_NETWORK_I_REST_REQUEST_RECEIVER_H_
#define STONKS_NETWORK_NETWORK_I_REST_REQUEST_RECEIVER_H_

#include <string>

#include "network_i_rest_request_handler.h"

namespace stonks::network {
/**
 * @brief Receives REST requests at the specified URI and redirects
 * them to the handler.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IRestRequestReceiver {
 public:
  virtual ~IRestRequestReceiver() = default;

  /**
   * @brief Start receiving REST requests on the specified URI
   * and forwarding them to the handler.
   */
  virtual void Receive(Uri uri, cpp::NnUp<IRestRequestHandler> handler) = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_REST_REQUEST_RECEIVER_H_
