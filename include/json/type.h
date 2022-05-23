#ifndef STONKS_JSON_TYPE_H_
#define STONKS_JSON_TYPE_H_

#include <cpprest/json.h>

#include <any>
#include <exception>
#include <optional>
#include <stdexcept>

#include "json_conversions.h"

namespace stonks::json {
/**
 * @brief Stores JSON and any conversions for the type.
 */
template <typename T>
class Type {
 public:
  [[nodiscard]] constexpr auto IsOptional() const -> bool { return false; }

  [[nodiscard]] auto ParseFromJson(const web::json::value &json) const
      -> std::optional<T> {
    return ::stonks::json::ParseFromJson<T>(json);
  }

  [[nodiscard]] auto ParseAnyFromJson(const web::json::value &json) const
      -> std::any {
    auto object = ParseFromJson(json);

    if (!object.has_value()) {
      return {};
    }

    return std::move(*object);
  }

  [[nodiscard]] auto ConvertAnyToJson(const std::any &data) const
      -> std::optional<web::json::value> {
    try {
      return ::stonks::json::ConvertToJson(std::any_cast<T>(data));
    } catch (const std::exception &) {
    }

    return std::nullopt;
  }

  [[nodiscard]] auto MakeNulloptAny() const -> std::any {
    return std::optional<T>{std::nullopt};
  }
};

/**
 * @brief Stores JSON and any conversions for the optional type.
 */
template <typename T>
class Type<std::optional<T>> {
 public:
  [[nodiscard]] constexpr auto IsOptional() const -> bool { return true; }

  [[nodiscard]] auto ParseAnyFromJson(const web::json::value &json) const
      -> std::any {
    if (json.is_null()) {
      return {};
    }

    const auto object = Type<T>{}.ParseFromJson(json);

    if (!object.has_value()) {
      return {};
    }

    return std::optional<T>{std::move(*object)};
  }

  [[nodiscard]] auto ConvertAnyToJson(const std::any &data) const
      -> std::optional<web::json::value> {
    if (!data.has_value()) {
      return web::json::value::null();
    }

    try {
      const auto &optional_data = std::any_cast<std::optional<T>>(data);

      if (!optional_data.has_value()) {
        return web::json::value::null();
      }

      return Type<T>{}.ConvertAnyToJson(*optional_data);
    } catch (const std::exception &) {
    }

    return std::nullopt;
  }

  [[nodiscard]] auto MakeNulloptAny() const -> std::any {
    return Type<T>{}.MakeNulloptAny();
  }
};
}  // namespace stonks::json

#endif  // STONKS_JSON_TYPE_H_
