#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_

#include "network_json.h"
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
  // NOLINTNEXTLINE(*-use-nodiscard)
  virtual auto SendRequestAndGetResponse(const Endpoint &endpoint,
                                         const RestRequestData &data) const
      -> Json = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
