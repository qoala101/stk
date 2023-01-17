#ifndef STONKS_NETWORK_NETWORK_REST_SERVER_H_
#define STONKS_NETWORK_NETWORK_REST_SERVER_H_

#include "cpp_meta_private_to.h"
#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"

namespace stonks::network {
class RestServerBuilder;

/**
 * @brief Handle which keeps REST receiver alive.
 */
class RestServer {
 public:
  RestServer(cpp::meta::PrivateTo<RestServerBuilder>,
             cpp::NnUp<IRestRequestReceiver> request_receiver);

 private:
  cpp::NnUp<IRestRequestReceiver> request_receiver_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_SERVER_H_
