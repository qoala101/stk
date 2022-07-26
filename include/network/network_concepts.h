#ifndef STONKS_NETWORK_NETWORK_CONCEPTS_H_
#define STONKS_NETWORK_NETWORK_CONCEPTS_H_

#include <polymorphic_value.h>

#include "network_i_json.h"
#include "network_json_basic_conversions.h"

namespace stonks::network {
template <typename T>
concept Parsable = requires(const IJson &json) {
  { ParseFromJson<T>(json) } -> std::same_as<T>;
};

template <typename T>
concept Convertible = requires(const T &t) {
  { ConvertToJson(t) } -> std::same_as<isocpp_p0201::polymorphic_value<IJson>>;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_CONCEPTS_H_
