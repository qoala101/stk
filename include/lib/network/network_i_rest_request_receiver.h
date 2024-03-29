/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_I_REST_REQUEST_RECEIVER_H_
#define VH_NETWORK_I_REST_REQUEST_RECEIVER_H_

#include <string>

#include "network_i_rest_request_handler.h"

namespace vh::network {
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
}  // namespace vh::network

#endif  // VH_NETWORK_I_REST_REQUEST_RECEIVER_H_
