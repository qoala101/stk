#ifndef STONKS_NETWORK_NETWORK_REST_CLIENT_H_
#define STONKS_NETWORK_NETWORK_REST_CLIENT_H_

#include <memory>
#include <string>
#include <string_view>

#include "network_i_rest_request_sender.h"
#include "network_types.h"
#include "not_null.hpp"

namespace stonks::network {
/**
 * @brief Client for the single REST resource with many endpoints.
 */
class RestClient {
 public:
  /**
   * @param base_uri Base resource URI.
   * @param request_sender Executes REST requests.
   */
  explicit RestClient(
      std::string_view base_uri,
      cpp::not_null<std::unique_ptr<IRestRequestSender>> request_sender);

  /**
   * @brief Sends REST request with result to the specified endpoint.
   */
  [[nodiscard]] auto CallAndGet(const Endpoint &endpoint,
                                const RestRequestData &data = {}) const
      -> Result;

  /**
   * @brief Sends REST request to the specified endpoint.
   */
  void Call(const Endpoint &endpoint, const RestRequestData &data = {}) const;

 private:
  std::string base_uri_{};
  cpp::not_null<std::unique_ptr<IRestRequestSender>> request_sender_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_CLIENT_H_
