/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORK_JSON_BASE_CONVERSIONS_H_
#define VH_NETWORK_JSON_BASE_CONVERSIONS_H_

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

namespace vh::network {
/**
 * @brief In order to make the type parsable, client must provide definition
 * of the operator for the type.
 */
template <typename T>
struct JsonParser {
  using Type = T;

  auto operator() [[nodiscard]] (const IJson &json) const -> Type;
};

/**
 * @remark To be defined by the library implementer.
 */
auto CreateNullJson [[nodiscard]] () -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
auto ConvertToJson [[nodiscard]] (bool value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
auto ConvertToJson [[nodiscard]] (int value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
auto ConvertToJson [[nodiscard]] (int64_t value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
auto ConvertToJson [[nodiscard]] (double value) -> cpp::Pv<IJson>;

/**
 * @remark To be defined by the library implementer.
 */
auto ConvertToJson [[nodiscard]] (std::string_view value) -> cpp::Pv<IJson>;
}  // namespace vh::network

#endif  // VH_NETWORK_JSON_BASE_CONVERSIONS_H_
