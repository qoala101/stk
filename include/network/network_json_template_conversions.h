#ifndef STONKS_NETWORK_NETWORK_JSON_TEMPLATE_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_TEMPLATE_CONVERSIONS_H_

#include "cpp_concepts.h"
#include "network_concepts.h"

/**
 * @file Conversions of standard templates.
 */

namespace stonks::network {
template <cpp::Vector T>
requires Parsable<typename T::value_type>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T {
  auto vector = T{};

  for (auto i = 0; i < json.GetSize(); ++i) {
    vector.emplace_back(
        ParseFromJson<typename T::value_type>(*json.GetChild(i)));
  }

  return vector;
}

template <cpp::Optional T>
requires Parsable<typename T::value_type>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T {
  if (json.IsNull()) {
    return std::nullopt;
  }

  return ParseFromJson<typename T::value_type>(json);
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(const std::vector<T> &value)
    -> cpp::Pv<IJson> {
  auto json = CreateNullJson();

  for (auto i = 0; i < value.size(); ++i) {
    json->SetChild(i, ConvertToJson(value[i]));
  }

  return json;
}

// TODO(vh): Changing typename to Convertible here leads to call to the bool
// overload when passing pointers.
template <typename T>
[[nodiscard]] auto ConvertToJson(T *value) -> cpp::Pv<IJson> {
  if (value == nullptr) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_TEMPLATE_CONVERSIONS_H_
