/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_TYPED_ENDPOINT_H_
#define VH_NETWORK_TYPED_ENDPOINT_H_

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

namespace vh::network {
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
  absl::flat_hash_map<std::string, ParseTypeCheck> params{};
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
}  // namespace vh::network

#endif  // VH_NETWORK_TYPED_ENDPOINT_H_
