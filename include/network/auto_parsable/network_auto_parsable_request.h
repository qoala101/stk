#ifndef STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_H_
#define STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_H_

#include <string_view>

#include "network_auto_parsable.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Wraps raw REST request providing convenient auto-parsable API
 * for its members.
 */
class AutoParsableRestRequest {
 public:
  explicit AutoParsableRestRequest(RestRequest rest_request);

  /**
   * @brief Gives request param by name by moving it into result.
   * @remark Same param should not be requested more than once.
   */
  [[nodiscard]] auto Param(std::string_view key) -> AutoParsable;

  /**
   * @brief Gives request body by moving it into result.
   * @remark Body should not be requested more than once.
   */
  [[nodiscard]] auto Body() -> AutoParsable;

 private:
  RestRequest rest_request_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_AUTO_PARSABLE_NETWORK_AUTO_PARSABLE_REQUEST_H_
