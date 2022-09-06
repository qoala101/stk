#ifndef STONKS_NETWORK_NETWORK_JSON_FACADES_H_
#define STONKS_NETWORK_NETWORK_JSON_FACADES_H_

#include "cpp_concepts.h"
#include "network_auto_parsable.h"
#include "network_concepts.h"
#include "network_i_json.h"

/**
 * @file Facades for the clients to build and parse JSONs.
 */

namespace stonks::network {
namespace details {
template <cpp::DecaysTo<IJson> Json, typename... Args>
[[nodiscard]] auto MakeFromJsonImpl(Json &&json, Args &&...args)
    -> AutoParsable {
  return AutoParsable{
      std::forward<Json>(json).GetChild(std::forward<Args>(args)...)};
}

inline void BuildJsonFromImpl(IJson &json) {}

template <std::convertible_to<std::string> Key, Convertible Value,
          typename... KeyValues>
void BuildJsonFromImpl(IJson &json, Key &&key, Value &&value,
                       KeyValues &&...key_values) {
  json.SetChild(std::forward<Key>(key),
                ConvertToJson(std::forward<Value>(value)));
  BuildJsonFromImpl(json, std::forward<KeyValues>(key_values)...);
}
}  // namespace details

/**
 * @brief Constructs object of type T by passing parsed JSON children
 * as constructor arguments.
 * @param keys List of JSON children names.
 */
template <typename T, cpp::DecaysTo<IJson> Json,
          std::convertible_to<std::string>... Keys>
[[nodiscard]] auto MakeFromJson(Json &&json, Keys &&...keys) -> T {
  return T{details::MakeFromJsonImpl(std::forward<Json>(json),
                                     std::forward<Keys>(keys))...};
}

/**
 * @brief Builds JSON with provided data.
 * @param key_values List of JSON children in form of
 * key1, value1, key2, value2, ..., keyN, valueN.
 */
template <typename... KeyValues>
[[nodiscard]] auto BuildJsonFrom(KeyValues &&...key_values) -> cpp::Pv<IJson> {
  auto json = stonks::network::CreateNullJson();
  details::BuildJsonFromImpl(*json, std::forward<KeyValues>(key_values)...);
  return json;
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_FACADES_H_
