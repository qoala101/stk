#ifndef STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_

#include <polymorphic_value.h>

#include "network_json.h"

/**
 * @file List of basic type conversions which should be provided by the network
 * library implementer. Client could then use those for conversion of client
 * types.
 */
namespace stonks::network {
template <typename T>
auto ParseFromJson(const IJson &json) -> T;

template <>
auto ParseFromJson(const IJson &json) -> int;

template <>
auto ParseFromJson(const IJson &json) -> int64_t;

template <>
auto ParseFromJson(const IJson &json) -> double;

template <>
auto ParseFromJson(const IJson &json) -> std::string;

auto ConvertToJson(int value) -> isocpp_p0201::polymorphic_value<IJson>;

auto ConvertToJson(int64_t value) -> isocpp_p0201::polymorphic_value<IJson>;

auto ConvertToJson(double value) -> isocpp_p0201::polymorphic_value<IJson>;

auto ConvertToJson(std::string_view value)
    -> isocpp_p0201::polymorphic_value<IJson>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
