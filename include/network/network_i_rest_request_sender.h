#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_

#include <utility>

#include "network_enums.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Interface for REST request execution.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IRestRequestSender {
 public:
  virtual ~IRestRequestSender() = default;

  /**
   * @brief Executes the request and returns response JSON.
   */
  [[nodiscard]] virtual auto SendRequestAndGetResponse(
      RestRequest request) const -> RestResponse = 0;

 protected:
  explicit IRestRequestSender() = default;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
