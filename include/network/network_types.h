#ifndef STONKS_NETWORK_NETWORK_TYPES_H_
#define STONKS_NETWORK_NETWORK_TYPES_H_

#include <compare>
#include <map>
#include <optional>
#include <string>

#include "ccutils_polymorphic_value.h"
#include "network_enums.h"
#include "network_i_json.h"

namespace stonks::network {
/**
 * @brief REST response body.
 */
using Result = std::optional<ccutils::Pv<IJson>>;

/**
 * @brief REST request param.
 */
using Param = ccutils::Pv<IJson>;

/**
 * @brief REST request params.
 */
using Params = std::map<std::string, Param>;

/**
 * @brief REST request headers.
 */
using Headers = std::map<std::string, std::string>;

/**
 * @brief REST request body.
 */
using Body = std::optional<ccutils::Pv<IJson>>;

/**
 * @brief HTTP endpoint.
 */
struct Endpoint {
  Method method{};
  std::string uri{};

 private:
  [[nodiscard]] friend auto operator==(const Endpoint &, const Endpoint &)
      -> bool = default;
  [[nodiscard]] friend auto operator<=>(const Endpoint &, const Endpoint &)
      -> std::partial_ordering = default;
};

/**
 * @brief Where and what to send via REST API call.
 */
struct RestRequest {
  [[nodiscard]] auto IsEmpty() const -> bool;

  Endpoint endpoint{};
  Params params{};
  Headers headers{};
  Body body{};
};

/**
 * @brief Result of REST API call.
 */
struct RestResponse {
  Status status{};
  Result result{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPES_H_
