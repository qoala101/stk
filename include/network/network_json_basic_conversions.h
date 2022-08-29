#ifndef STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_

#include <polymorphic_value.h>

#include <cstddef>
#include <exception>
#include <stdexcept>

#include "cpp_message_exception.h"
#include "cpp_not_null.h"
#include "network_i_json.h"
#include "not_null.hpp"

/**
 * @file List of basic type conversions which should be provided by the network
 * library implementer. Client could then use those for conversion of client
 * types.
 */

namespace stonks::network {
[[nodiscard]] auto CreateNullJson() -> cpp::Pv<IJson>;

template <typename T>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> bool;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> int;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> int64_t;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> double;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> std::string;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> cpp::MessageException;

[[nodiscard]] auto ConvertToJson(bool value) -> cpp::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(int value) -> cpp::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(int64_t value) -> cpp::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(double value) -> cpp::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(const char *value) -> cpp::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(std::string_view value) -> cpp::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(const std::exception &value) -> cpp::Pv<IJson>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
