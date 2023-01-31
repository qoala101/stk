#ifndef VH_NETWORK_REST_SERVER_H_
#define VH_NETWORK_REST_SERVER_H_

#include "cpp_not_null.h"
#include "network_i_rest_request_receiver.h"

namespace vh::network {
/**
 * @brief Handle which keeps REST receiver alive.
 */
class RestServer {
 public:
  explicit RestServer(cpp::NnUp<IRestRequestReceiver> request_receiver);

 private:
  cpp::NnUp<IRestRequestReceiver> request_receiver_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_REST_SERVER_H_
