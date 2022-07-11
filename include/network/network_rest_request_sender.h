#ifndef STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
#define STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_

#include <polymorphic_value.h>

#include <memory>

#include "network_json.h"
#include "network_types.h"
#include "not_null.hpp"

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
  // NOLINTNEXTLINE(*-use-nodiscard)
  virtual auto SendRequestAndGetResponse(const RestRequestData &data) const
      -> isocpp_p0201::polymorphic_value<IJson> = 0;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_REST_REQUEST_SENDER_H_
