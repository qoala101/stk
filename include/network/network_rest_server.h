#ifndef STONKS_NETWORK_NETWORK_REST_SERVER_H_
#define STONKS_NETWORK_NETWORK_REST_SERVER_H_

#include "cpp_expose_private_constructors.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"

namespace stonks::network {
class RestServerBuilder;

/**
 * @brief Handle which keeps REST receiver alive.
 */
class RestServer {
 private:
  friend class cpp::ExposePrivateConstructorsTo<RestServerBuilder, RestServer>;

  explicit RestServer(cpp::NnUp<IRestRequestReceiver> request_receiver);

  cpp::NnUp<IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_SERVER_H_
