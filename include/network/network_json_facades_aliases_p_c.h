#ifndef STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_
#define STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_

#include "network_i_json.h"
#include "network_json_facades.h"

/**
 * @file Short aliases for JSON facades.
 */

template <typename T, typename... Args>
[[nodiscard]] auto P(Args &&...args) -> T {
  return stonks::network::MakeFromJson<T>(std::forward<Args>(args)...);
}

template <typename... Args>
[[nodiscard]] auto C(Args &&...args)
    -> stonks::cpp::Pv<stonks::network::IJson> {
  return stonks::network::BuildJsonFrom(std::forward<Args>(args)...);
}

#endif  // STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_
