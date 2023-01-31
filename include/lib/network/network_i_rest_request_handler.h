#ifndef STONKS_NETWORK_NETWORK_I_REST_REQUEST_HANDLER_H_
#define STONKS_NETWORK_NETWORK_I_REST_REQUEST_HANDLER_H_

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

#endif  // STONKS_NETWORK_NETWORK_I_REST_REQUEST_HANDLER_H_
