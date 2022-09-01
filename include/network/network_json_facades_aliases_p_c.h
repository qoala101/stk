#ifndef STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_
#define STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_

#include "network_i_json.h"
#include "network_json_facades.h"

/**
 * @file Short aliases for JSON facades.
 */

template <typename T>
[[nodiscard]] auto P(auto &&...args) -> T {
  return stonks::network::MakeFromJson<T>(
      std::forward<decltype(args)>(args)...);
}

[[nodiscard]] auto C(auto &&...args)
    -> stonks::cpp::Pv<stonks::network::IJson> {
  return stonks::network::BuildJsonFrom(std::forward<decltype(args)>(args)...);
}

#endif  // STONKS_NETWORK_NETWORK_JSON_FACADES_ALIASES_P_C_H_
