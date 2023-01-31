#ifndef VH_NETWORK_AUTO_PARSABLE_REQUEST_H_
#define VH_NETWORK_AUTO_PARSABLE_REQUEST_H_

#include <string_view>

#include "cpp_optional.h"
#include "network_auto_parsable.h"
#include "network_types.h"

namespace vh::network {
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
  auto Param [[nodiscard]] (std::string_view key) -> AutoParsable;

  /**
   * @brief Gives request body by moving it into result.
   * @remark Body should not be requested more than once.
   */
  auto Body [[nodiscard]] () -> AutoParsable;

 private:
  Params params_{};
  cpp::Opt<network::Body> body_{};
};
}  // namespace vh::network

#endif  // VH_NETWORK_AUTO_PARSABLE_REQUEST_H_
