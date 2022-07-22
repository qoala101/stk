#ifndef STONKS_NETWORK_NETWORK_TYPES_H_
#define STONKS_NETWORK_NETWORK_TYPES_H_

#include <compare>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <variant>

#include "network_enums.h"
#include "network_i_json.h"

namespace stonks::network {
using Result = std::optional<isocpp_p0201::polymorphic_value<IJson>>;
using Param = isocpp_p0201::polymorphic_value<IJson>;
using Params = std::map<std::string, Param>;
using Headers = std::map<std::string, std::string>;
using Body = std::optional<isocpp_p0201::polymorphic_value<IJson>>;

/**
 * @brief HTTP endpoint.
 */
struct Endpoint {
  Method method{};
  std::string uri{};

 private:
  friend auto operator==(const Endpoint &, const Endpoint &) -> bool = default;
  friend auto operator<=>(const Endpoint &, const Endpoint &)
      -> std::partial_ordering = default;
};

/**
 * @brief Where and what to send via REST API call.
 */
struct RestRequest {
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

/**
 * @brief Handles REST request and returns response.
 */
using RestRequestHandler = std::function<RestResponse(RestRequest)>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPES_H_
