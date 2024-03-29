/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_I_REST_REQUEST_HANDLER_H_
#define VH_NETWORK_I_REST_REQUEST_HANDLER_H_

#include <cppcoro/task.hpp>

#include "network_types.h"

namespace vh::network {
/**
 * @brief Handles REST request.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IRestRequestHandler {
 public:
  virtual ~IRestRequestHandler() = default;

  /**
   * @return Response for the given request.
   */
  virtual auto HandleRequestAndGiveResponse [[nodiscard]] (RestRequest request)
  -> cppcoro::task<RestResponse> = 0;
};
}  // namespace vh::network

#endif  // VH_NETWORK_I_REST_REQUEST_HANDLER_H_
