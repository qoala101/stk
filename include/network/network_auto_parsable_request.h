#ifndef STONKS_NETWORK_NETWORK_AUTO_PARSABLE_REQUEST_H_
#define STONKS_NETWORK_NETWORK_AUTO_PARSABLE_REQUEST_H_

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

  AutoParsableRestRequest(const AutoParsableRestRequest &) = delete;
  AutoParsableRestRequest(AutoParsableRestRequest &&) noexcept = default;

  auto operator=(const AutoParsableRestRequest &)
      -> AutoParsableRestRequest & = delete;
  auto operator=(AutoParsableRestRequest &&) noexcept
      -> AutoParsableRestRequest & = default;

  ~AutoParsableRestRequest() = default;

  /**
   * @brief Gives request param by name.
   */
  [[nodiscard]] auto Param(std::string_view key) const & -> AutoParsable;

  /**
   * @copydoc Param
   */
  [[nodiscard]] auto Param(std::string_view key) && -> AutoParsable;

  /**
   * @brief Gives request body.
   */
  [[nodiscard]] auto Body() const & -> AutoParsable;

  /**
   * @copydoc Body
   */
  [[nodiscard]] auto Body() && -> AutoParsable;

 private:
  [[nodiscard]] static auto ParamImpl(auto &&t, std::string_view key)
      -> AutoParsable;

  [[nodiscard]] static auto BodyImpl(auto &&t) -> AutoParsable;

  RestRequest rest_request_;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_AUTO_PARSABLE_REQUEST_H_
