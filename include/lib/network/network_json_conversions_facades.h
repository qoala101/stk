#ifndef STONKS_NETWORK_NETWORK_JSON_CONVERSIONS_FACADES_H_
#define STONKS_NETWORK_NETWORK_JSON_CONVERSIONS_FACADES_H_

#include <utility>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_for_each_arg.h"
#include "network_auto_parsable.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "network_json_common_conversions.h"

/**
 * @file Facades for the clients to build and parse JSONs.
 */

namespace vh::network {
/**
 * @brief Alias for JSON parser operator which converts JSON to an object.
 */
template <Parsable T>
auto ParseFromJson [[nodiscard]] (const IJson &json) -> T {
  return JsonParser<T>{}(json);
}

/**
 * @brief Constructs object of type T by passing parsed JSON children
 * as constructor arguments.
 * @param keys List of JSON children names.
 */
template <Parsable T, typename... Keys>
auto MakeFromJson [[nodiscard]] (cpp::Is<IJson> auto &json, Keys &&...keys) {
  return T{AutoParsable{json.GetChild(std::forward<Keys>(keys))}...};
}

/**
 * @brief Shortcut for common use case of parsing JSON child.
 */
template <Parsable T, typename Key>
auto ParseFromJsonChild [[nodiscard]] (const IJson &json, Key &&child_key) {
  return ParseFromJson<T>(*json.GetChild(std::forward<Key>(child_key)));
}

/**
 * @brief Builds JSON with provided data.
 * @param key_values List of JSON children in form of
 * key1, value1, key2, value2, ..., keyN, valueN.
 */
template <typename... KeyValues>
auto BuildJsonFrom [[nodiscard]] (KeyValues &&...key_values) {
  auto json = vh::network::CreateNullJson();

  cpp::ForEachArg<2>(
      [&json]<typename Key, typename Value>(Key &&key, Value &&value, auto) {
        json->SetChild(std::forward<Key>(key),
                       ConvertToJson(std::forward<Value>(value)));
      },
      std::forward<KeyValues>(key_values)...);

  return json;
}
}  // namespace vh::network

#endif  // STONKS_NETWORK_NETWORK_JSON_CONVERSIONS_FACADES_H_
