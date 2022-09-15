#ifndef STONKS_NETWORK_NETWORK_REST_CLIENT_H_
#define STONKS_NETWORK_NETWORK_REST_CLIENT_H_

#include "cpp_not_null.h"
#include "network_i_rest_request_sender.h"
#include "network_rest_client_request_builder.h"
#include "network_typed_endpoint.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Client for the REST resource with many endpoints.
 */
class RestClient {
 public:
  /**
   * @param request_sender Executes REST requests.
   * @param base_uri Base resource URI.
   */
  RestClient(cpp::NnSp<IRestRequestSender> request_sender, Uri base_uri);

  /**
   * @brief Creates request builder which provides a further API for request
   * sending.
   * @param endpoint Endpoint relative to the base URI.
   */
  [[nodiscard]] auto Call(TypedEndpoint endpoint) const
      -> rest_client::RequestBuilder;

 private:
  cpp::NnSp<IRestRequestSender> request_sender_;
  Uri base_uri_{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_CLIENT_H_
