#ifndef STONKS_NETWORK_NETWORK_TYPES_H_
#define STONKS_NETWORK_NETWORK_TYPES_H_

#include <compare>
#include <map>
#include <string>

#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_enums.h"
#include "network_i_json.h"

namespace stonks::network {
/**
 * @brief Full or partial HTTP URI.
 */
struct Uri : public cpp::TypedStruct<std::string> {};

/**
 * @brief REST response body.
 */
using Result = cpp::Opt<cpp::Pv<IJson>>;

/**
 * @brief REST request param.
 */
using Param = cpp::Pv<IJson>;

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
using Body = cpp::Opt<cpp::Pv<IJson>>;

/**
 * @brief HTTP endpoint.
 */
struct Endpoint {
  Method method{};
  Uri uri{};

 private:
  friend auto operator== [[nodiscard]] (const Endpoint &, const Endpoint &)
  -> bool = default;
  friend auto operator<=> [[nodiscard]] (const Endpoint &, const Endpoint &)
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
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_TYPES_H_
