#ifndef STONKS_NETWORK_NETWORK_JSON_FACADES_H_
#define STONKS_NETWORK_NETWORK_JSON_FACADES_H_

#include "network_auto_parsable.h"
#include "network_concepts.h"
#include "network_i_json.h"

/**
 * @file Facades for the clients to build and parse JSONs.
 */

namespace stonks::network {
namespace details {
[[nodiscard]] auto MakeFromJsonImpl(const IJson &json, auto &&...args)
    -> AutoParsable {
  return AutoParsable{json.GetChild(std::forward<decltype(args)>(args)...)};
}

inline void BuildJsonFromImpl(IJson &json) {}

void BuildJsonFromImpl(IJson &json, std::convertible_to<std::string> auto &&key,
                       Convertible auto &&value, auto &&...key_values) {
  json.SetChild(std::forward<decltype(key)>(key),
                ConvertToJson(std::forward<decltype(value)>(value)));
  BuildJsonFromImpl(json, std::forward<decltype(key_values)>(key_values)...);
}
}  // namespace details

/**
 * @brief Constructs object of type T by passing parsed JSON children
 * as constructor arguments.
 * @param keys List of JSON children names.
 */
template <typename T>
[[nodiscard]] auto MakeFromJson(const IJson &json, auto &&...keys) -> T {
  return T{details::MakeFromJsonImpl(json, keys)...};
}

/**
 * @brief Builds JSON with provided data.
 * @param key_values List of JSON children in form of
 * key1, value1, key2, value2, ..., keyN, valueN.
 */
[[nodiscard]] auto BuildJsonFrom(auto &&...key_values) -> cpp::Pv<IJson> {
  auto json = stonks::network::CreateNullJson();
  details::BuildJsonFromImpl(*json,
                             std::forward<decltype(key_values)>(key_values)...);
  return json;
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_FACADES_H_
