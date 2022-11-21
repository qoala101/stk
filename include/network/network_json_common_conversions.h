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
#include "network_json_base_conversions.h"

/**
 * @file Common conversions based on the base ones and standard templates.
 */

namespace stonks::network {
template <Parsable T>
auto ParseFromJson [[nodiscard]] (const IJson &json) -> T;

namespace detail {
template <cpp::Variant T, unsigned kIndex>
  requires Parsable<std::variant_alternative_t<kIndex, T>>
struct JsonVariantParser {
  auto operator() [[nodiscard]] (const IJson &json) const -> T try {
    return ParseFromJson<std::variant_alternative_t<kIndex, T>>(json);
  } catch (const std::exception &) {
    if constexpr (kIndex > 0) {
      return JsonVariantParser<T, kIndex - 1>{}(json);
    } else {
      throw cpp::MessageException{"Couldn't parse any variant type"};
    }
  }
};
}  // namespace detail

/**
 * @remark Added to avoid implicit conversion to bool.
 */
auto ConvertToJson [[nodiscard]] (const char *value) -> cpp::Pv<IJson>;

/**
 * @brief Gives null JSON.
 */
auto ConvertToJson [[nodiscard]] (std::monostate value) -> cpp::Pv<IJson>;

/**
 * @brief Converts to string representation.
 */
auto ConvertToJson [[nodiscard]] (const std::exception &value)
-> cpp::Pv<IJson>;

/**
 * @brief Parses descendants of typed structs.
 */
template <cpp::IsTypedStruct T>
  requires Parsable<typename T::ValueType>
struct JsonParser<T> {
  using Type = T;

  auto operator() [[nodiscard]] (const IJson &json) const -> Type {
    return Type{ParseFromJson<typename Type::ValueType>(json)};
  }
};

/**
 * @brief Converts descendants of typed structs.
 */
template <Convertible T>
auto ConvertToJson [[nodiscard]] (const cpp::TypedStruct<T> &value) {
  return ConvertToJson(*value);
}

/**
 * @brief Parses variant type.
 */
template <cpp::Variant T>
struct JsonParser<T> {
  using Type = T;

  auto operator() [[nodiscard]] (const IJson &json) const -> Type {
    return detail::JsonVariantParser<Type, std::variant_size_v<Type> - 1>{}(
        json);
  }
};

/**
 * @brief Converts variants.
 */
template <Convertible... Ts>
auto ConvertToJson [[nodiscard]] (const std::variant<Ts...> &value) {
  return std::visit([](const auto &v) { return ConvertToJson(v); }, value);
}

/**
 * @brief Parses optional values.
 */
template <Parsable T>
struct JsonParser<cpp::Opt<T>> {
  using Type = cpp::Opt<T>;

  auto operator() [[nodiscard]] (const IJson &json) const -> Type {
    if (json.IsNull()) {
      return std::nullopt;
    }

    return ParseFromJson<T>(json);
  }
};

/**
 * @brief Converts optional values.
 */
template <Convertible T>
auto ConvertToJson [[nodiscard]] (const cpp::Opt<T> &value) {
  if (!value.has_value()) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}

/**
 * @brief Parses vectors.
 */
template <Parsable T>
struct JsonParser<std::vector<T>> {
  using Type = std::vector<T>;

  auto operator() [[nodiscard]] (const IJson &json) const -> Type {
    auto vector = Type{};

    for (auto i = 0; i < json.GetSize(); ++i) {
      vector.emplace_back(ParseFromJson<T>(*json.GetChild(i)));
    }

    return vector;
  }
};

/**
 * @brief Converts vectors.
 */
template <Convertible T>
auto ConvertToJson [[nodiscard]] (const std::vector<T> &value) {
  auto json = CreateNullJson();

  for (auto i = 0; i < gsl::narrow_cast<int>(value.size()); ++i) {
    json->SetChild(i, ConvertToJson(value[i]));
  }

  return json;
}

/**
 * @brief Converts pointers.
 * @remark Use optional values when need to parse nullable pointers.
 */
template <Convertible T>
auto ConvertToJson [[nodiscard]] (T *value) {
  if (value == nullptr) {
    return CreateNullJson();
  }

  return ConvertToJson(*value);
}
}  // namespace stonks::network

#endif  // STONKS_NETWORK_NETWORK_JSON_COMMON_CONVERSIONS_H_
