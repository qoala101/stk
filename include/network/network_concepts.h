#ifndef STONKS_NETWORK_NETWORK_CONCEPTS_H_
#define STONKS_NETWORK_NETWORK_CONCEPTS_H_

#include <polymorphic_value.h>

#include "network_i_json.h"

namespace stonks::network {
template <typename T>
concept Parsable = requires(const IJson &json) {
  ParseFromJson<T>(json);
};

template <typename T>
concept Convertible = requires(const T &t) {
  { ConvertToJson(t) } -> std::same_as<isocpp_p0201::polymorphic_value<IJson>>;
};
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_CONCEPTS_H_
