#ifndef STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_
#define STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_

#include "network_i_json.h"
#include "network_json_common_conversions.h"
#include "network_json_facades.h"

/**
 * @file Short aliases for JSON facades.
 */

template <stonks::network::Parsable T,
          stonks::cpp::DecaysTo<stonks::network::IJson> Json>
[[nodiscard]] auto P(Json &&json) -> stonks::network::Parsable auto{
  return stonks::network::ParseFromJson<T>(std::forward<Json>(json));
}

template <stonks::network::Parsable T, typename... Args>
[[nodiscard]] auto P(Args &&...args) -> T {
  return stonks::network::MakeFromJson<T>(std::forward<Args>(args)...);
}

template <stonks::network::Convertible Value>
[[nodiscard]] auto C(Value &&value) -> stonks::cpp::Pv<stonks::network::IJson> {
  return stonks::network::ConvertToJson(std::forward<Value>(value));
}

template <typename... Args>
[[nodiscard]] auto C(Args &&...args)
    -> stonks::cpp::Pv<stonks::network::IJson> {
  return stonks::network::BuildJsonFrom(std::forward<Args>(args)...);
}

#endif  // STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_
