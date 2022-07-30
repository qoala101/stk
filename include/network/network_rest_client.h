#ifndef STONKS_NETWORK_NETWORK_REST_CLIENT_H_
#define STONKS_NETWORK_NETWORK_REST_CLIENT_H_

#include <memory>
#include <string>
#include <string_view>

#include "network_i_rest_request_sender.h"
#include "network_rest_client_request_builder.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
/**
 * @brief Client for the REST resource with many endpoints.
 */
class RestClient {
 public:
  /**
   * @param base_uri Base resource URI.
   * @param request_sender Executes REST requests.
   */
  explicit RestClient(
      std::string base_uri,
      cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender);

  /**
   * @brief Creates request builder which provides a further API for request
   * sending.
   * @param endpoint Endpoint relative to the base URI.
   */
  [[nodiscard]] auto Call(Endpoint endpoint) const
      -> rest_client::RequestBuilder;

 private:
  std::string base_uri_{};
  cpp::not_null<std::shared_ptr<IRestRequestSender>> request_sender_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_CLIENT_H_
