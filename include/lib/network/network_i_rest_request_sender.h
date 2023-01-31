#ifndef STONKS_NETWORK_NETWORK_I_REST_REQUEST_SENDER_H_
#define STONKS_NETWORK_NETWORK_I_REST_REQUEST_SENDER_H_

#include <cppcoro/task.hpp>
#include <utility>

#include "network_types.h"

namespace stonks::network {
/**
 * @brief Executes REST API calls by sending the requests and receiving
 * responses.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IRestRequestSender {
 public:
  virtual ~IRestRequestSender() = default;

  /**
   * @brief Executes the request and returns response JSON.
   */
  virtual auto SendRequestAndGetResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_REST_REQUEST_SENDER_H_
