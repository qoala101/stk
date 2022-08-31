#ifndef STONKS_NETWORK_NETWORK_I_REST_REQUEST_HANDLER_H_
#define STONKS_NETWORK_NETWORK_I_REST_REQUEST_HANDLER_H_

#include "network_types.h"

namespace stonks::network {
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
  [[nodiscard]] virtual auto HandleRequestAndGiveResponse(
      RestRequest request) const -> RestResponse = 0;

 protected:
  IRestRequestHandler() = default;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_I_REST_REQUEST_HANDLER_H_
