/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_REST_CLIENT_H_
#define VH_NETWORK_REST_CLIENT_H_

#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"
#include "network_rest_client_request_builder.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace vh::network {
/**
 * @brief Client for the REST resource with many endpoints.
 */
class RestClient {
 public:
  /**
   * @param base_uri Base resource URI.
   */
  RestClient(Uri base_uri, cpp::NnUp<IRestRequestSender> request_sender);

  /**
   * @brief Creates request builder which provides a further API for request
   * sending.
   * @param endpoint Endpoint relative to the base URI.
   */
  auto Call [[nodiscard]] (TypedEndpoint endpoint) const
      -> rest_client::RequestBuilder;

 private:
  Uri base_uri_{};
  cpp::NnSp<IRestRequestSender> request_sender_;
};
}  // namespace vh::network

#endif  // VH_NETWORK_REST_CLIENT_H_
