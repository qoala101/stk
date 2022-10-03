#ifndef STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_
#define STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_

#include <bits/exception.h>
#include <polymorphic_value.h>

#include <gsl/assert>
#include <gsl/util>
#include <optional>
#include <string>
#include <variant>
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
namespace detail {
template <cpp::Variant T, int Index>
  requires Parsable<std::variant_alternative_t<Index, T>>
[[nodiscard]] auto ParseVariantFromJson(const IJson &json) -> T try {
  return ParseFromJson<std::variant_alternative_t<Index, T>>(json);
} catch (const std::exception &) {
  if constexpr (Index > 0) {
    return ParseVariantFromJson<T, Index - 1>(json);
  } else {
    throw cpp::MessageException{"Couldn't parse any variant type"};
  }
}
}  // namespace detail

[[nodiscard]] auto ConvertToJson(const char *value) -> cpp::Pv<IJson>;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> cpp::MessageException;
[[nodiscard]] auto ConvertToJson(const std::exception &value) -> cpp::Pv<IJson>;

template <>
[[nodiscard]] auto ParseFromJson(const IJson &json) -> std::monostate;
[[nodiscard]] auto ConvertToJson(std::monostate value) -> cpp::Pv<IJson>;

template <cpp::IsTypedStruct T>
  requires Parsable<typename T::ValueType>
[[nodiscard]] auto ParseFromJson(const IJson &json) {
  return T{ParseFromJson<typename T::ValueType>(json)};
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(const cpp::TypedStruct<T> &value) {
  return ConvertToJson(value.value);
}

template <cpp::Variant T>
[[nodiscard]] auto ParseFromJson(const IJson &json) {
  return detail::ParseVariantFromJson<T, std::variant_size_v<T> - 1>(json);
}

template <Convertible... Ts>
[[nodiscard]] auto ConvertToJson(const std::variant<Ts...> &value) {
  return std::visit([](const auto &v) { return ConvertToJson(v); }, value);
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
[[nodiscard]] auto ConvertToJson(const cpp::Opt<T> &value) {
  if (!value.has_value()) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}

template <cpp::Vector T>
  requires Parsable<typename T::value_type>
[[nodiscard]] auto ParseFromJson(const IJson &json) {
  auto vector = T{};

  for (auto i = 0; i < json.GetSize(); ++i) {
    vector.emplace_back(
        ParseFromJson<typename T::value_type>(*json.GetChild(i)));
  }

  Ensures(gsl::narrow_cast<int>(vector.size()) == json.GetSize());
  return vector;
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(const std::vector<T> &value) {
  auto json = CreateNullJson();

  for (auto i = 0; i < gsl::narrow_cast<int>(value.size()); ++i) {
    json->SetChild(i, ConvertToJson(value[i]));
  }

  Ensures(json->GetSize() == gsl::narrow_cast<int>(value.size()));
  return json;
}

template <Convertible T>
[[nodiscard]] auto ConvertToJson(T *value) {
  if (value == nullptr) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_
