#ifndef STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_H_
#define STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_H_

#include <function2/function2.hpp>
#include <map>
#include <string>

#include "cpp_optional.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"
#include "network_json_conversions_facades.h"
#include "network_types.h"

namespace stonks::network {
/**
 * @brief Tries to parse JSON.
 * @throw Rethrows parsing error if any happens.
 */
using ParseTypeCheck = fu2::function<void(const IJson &) const>;

/**
 * @brief Convenience API to create parse type check.
 */
template <Parsable T>
auto ExpectedType [[nodiscard]] () {
  return [](const auto &json) { std::ignore = ParseFromJson<T>(json); };
}

/**
 * @brief Expected request and response types.
 */
struct EndpointTypes {
  std::map<std::string, ParseTypeCheck> params{};
  ParseTypeCheck body{};
  ParseTypeCheck result{};
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

#endif  // STONKS_NETWORK_TYPED_ENDPOINT_NETWORK_TYPED_ENDPOINT_H_
