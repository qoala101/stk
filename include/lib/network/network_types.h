#ifndef VH_NETWORK_TYPES_H_
#define VH_NETWORK_TYPES_H_

#include <absl/container/flat_hash_map.h>

#include <compare>
#include <map>
#include <string>

#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "cpp_typed_struct.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"

namespace vh::network {
/**
 * @brief Full or partial HTTP URI.
 */
struct Uri : public cpp::TypedStruct<std::string> {};

/**
 * @brief REST response body.
 */
using Result = cpp::Pv<IJson>;

/**
 * @brief REST request param.
 */
using Param = cpp::Pv<IJson>;

/**
 * @brief REST request params.
 */
using Params = absl::flat_hash_map<std::string, Param>;

/**
 * @brief REST request headers.
 */
using Headers = absl::flat_hash_map<std::string, std::string>;

/**
 * @brief REST request body.
 */
using Body = cpp::Pv<IJson>;

/**
 * @brief REST request method.
 */
enum class Method { kGet, kPost, kDelete, kOther };

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
 * @remark Required for Abseil maps to work.
 */
template <typename Hash>
auto AbslHashValue [[nodiscard]] (Hash hash, const Endpoint &value) {
  return Hash::combine(std::move(hash), value.method, *value.uri);
}

/**
 * @brief Where and what to send via REST API call.
 */
struct RestRequest {
  Endpoint endpoint{};
  Params params{};
  Headers headers{};
  // TODO(vh): Find a way to initialize the value with null JSON by default.
  Body body{CreateNullJson()};
};

/**
 * @brief REST response status.
 */
enum class Status {
  kOtherInfo,
  kOtherSuccess,
  kOk,
  kOtherRedirection,
  kOtherClientError,
  kBadRequest,
  kNotFound,
  kOtherServerError,
  kInternalError
};

/**
 * @brief Result of REST API call.
 */
struct RestResponse {
  Status status{};
  // TODO(vh): Find a way to initialize the value with null JSON by default.
  Result result{CreateNullJson()};
};
}  // namespace vh::network

#endif  // VH_NETWORK_TYPES_H_
