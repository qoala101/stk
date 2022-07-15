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
   * @remark Destination is specified during construction.
   */
  [[nodiscard]] virtual auto SendRequestAndGetResponse(
      const RestRequestData &data) const -> std::pair<Status, Result> = 0;

  /**
   * @brief Executes the request.
   * @remark Destination is specified during construction.
   */
  void SendRequest(const RestRequestData &data) const;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
