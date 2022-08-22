#ifndef STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_

#include <polymorphic_value.h>

#include <exception>
#include <stdexcept>

#include "ccutils_not_null.h"
#include "network_i_json.h"
#include "not_null.hpp"

/**
 * @file List of basic type conversions which should be provided by the network
 * library implementer. Client could then use those for conversion of client
 * types.
 */
namespace stonks::network {
template <typename T>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> int;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> int64_t;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> double;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> std::string;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json)
    -> ccutils::NnUp<std::exception>;

[[nodiscard]] auto ConvertToJson(int value) -> ccutils::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(int64_t value) -> ccutils::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(double value) -> ccutils::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(std::string_view value) -> ccutils::Pv<IJson>;

[[nodiscard]] auto ConvertToJson(const std::exception &value)
    -> ccutils::Pv<IJson>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
