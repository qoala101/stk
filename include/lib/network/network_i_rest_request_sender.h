/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_I_REST_REQUEST_SENDER_H_
#define VH_NETWORK_I_REST_REQUEST_SENDER_H_

#include <cppcoro/task.hpp>
#include <utility>

#include "network_types.h"

namespace vh::network {
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
}  // namespace vh::network

#endif  // VH_NETWORK_I_REST_REQUEST_SENDER_H_
