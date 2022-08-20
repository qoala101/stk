#ifndef STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_H_
#define STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_H_

#include <polymorphic_value.h>

#include <compare>
#include <functional>
#include <gsl/assert>
#include <map>
#include <optional>
#include <string>
#include <tuple>

#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Tries to parse JSON.
 * @throw Rethrows parsing error if any happens.
 */
using ParseTypeCheck = std::function<void(const IJson &)>;

/**
 * @brief Convenience API to create parse type check.
 */
template <Parsable T>
auto ExpectedType() -> ParseTypeCheck {
  return [](const IJson &json) { std::ignore = ParseFromJson<T>(json); };
}

/**
 * @brief Expected request and response types.
 */
struct EndpointTypes {
  std::map<std::string, ParseTypeCheck> params{};
  std::optional<ParseTypeCheck> body{};
  std::optional<ParseTypeCheck> result{};
};

/**
 * @brief Endpoint which request and response to be validated
 * to have expected types prior to processing.
 */
struct TypedEndpoint {
  Endpoint endpoint{};
  EndpointTypes expected_types{};
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPED_ENDPOINT_H_
