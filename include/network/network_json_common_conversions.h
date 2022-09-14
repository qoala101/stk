#ifndef STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_

#include <bits/exception.h>
#include <polymorphic_value.h>

#include <gsl/assert>
#include <optional>
#include <vector>

#include "cpp_concepts.h"  // IWYU pragma: keep
#include "cpp_message_exception.h"
#include "cpp_optional.h"
#include "cpp_polymorphic_value.h"
#include "network_concepts.h"  // IWYU pragma: keep
#include "network_i_json.h"
#include "network_json_basic_conversions.h"

/**
 * @file Common conversions based on basic ones and standard templates.
 */

namespace stonks::network {
[[nodiscard]] auto ConvertToJson(const char *value) -> cpp::Pv<IJson>;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> cpp::MessageException;
[[nodiscard]] auto ConvertToJson(const std::exception &value) -> cpp::Pv<IJson>;

template <cpp::IsTypedStruct T>
requires Parsable<typename T::ValueType>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T {
  return {ParseFromJson<typename T::ValueType>(json)};
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(const cpp::TypedStruct<T> &value)
    -> cpp::Pv<IJson> {
  return ConvertToJson(value.value);
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
[[nodiscard]] auto ConvertToJson(const cpp::Opt<T> &value) -> cpp::Pv<IJson> {
  if (!value.has_value()) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}

template <cpp::Vector T>
requires Parsable<typename T::value_type>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> T {
  auto vector = T{};

  for (auto i = 0; i < json.GetSize(); ++i) {
    vector.emplace_back(
        ParseFromJson<typename T::value_type>(*json.GetChild(i)));
  }

  Ensures(vector.size() == json.GetSize());
  return vector;
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(const std::vector<T> &value)
    -> cpp::Pv<IJson> {
  auto json = CreateNullJson();

  for (auto i = 0; i < value.size(); ++i) {
    json->SetChild(i, ConvertToJson(value[i]));
  }

  Ensures(json->GetSize() == value.size());
  return json;
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(T *value) -> cpp::Pv<IJson> {
  if (value == nullptr) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_
