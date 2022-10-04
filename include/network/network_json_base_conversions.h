#ifndef STONKS_NETWORK_NETWORK_JSON_BASE_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_BASE_CONVERSIONS_H_

#include <polymorphic_value.h>

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string_view>

#include "network_i_json.h"

/**
 * @file List of simple types conversions which should be provided
 * by the network library implementer. Client could then use those
 * for conversion of user types.
 */

namespace stonks::network {
/**
 * @brief In order to make the type parsable, client must provide definition
 * of the operator for the type.
 */
template <typename T>
struct JsonParser {
  using Type = T;

  [[nodiscard]] auto operator()(const IJson &json) const -> Type;
};

/**
 * @remark To be defined by the library implementer.
 */
[[nodiscard]] auto CreateNullJson() -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
[[nodiscard]] auto ConvertToJson(bool value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
[[nodiscard]] auto ConvertToJson(int value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
[[nodiscard]] auto ConvertToJson(int64_t value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
[[nodiscard]] auto ConvertToJson(double value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
[[nodiscard]] auto ConvertToJson(std::string_view value) -> cpp::Pv<IJson>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_BASE_CONVERSIONS_H_
