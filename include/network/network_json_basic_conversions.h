#ifndef STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_

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
auto ParseFromJson(const stonks::network::IJson &json) -> int;

template <>
auto ParseFromJson(const stonks::network::IJson &json) -> int64_t;

template <>
auto ParseFromJson(const stonks::network::IJson &json) -> double;

template <>
auto ParseFromJson(const stonks::network::IJson &json) -> std::string;

auto ConvertToJson(const int &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>>;

auto ConvertToJson(const int64_t &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>>;

auto ConvertToJson(const double &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>>;

auto ConvertToJson(const std::string &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>>;
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_BASIC_CONVERSIONS_H_
