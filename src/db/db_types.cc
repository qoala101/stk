#include "db_types.h"

namespace stonks::db {
std::optional<int64_t> Value::GetInt64() const {
  return std::visit(
      [](const auto& variant) -> std::optional<int64_t> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int64_t>) {
          return variant;
        }

        return std::nullopt;
      },
      value);
}

std::optional<double> Value::GetDouble() const {
  return std::visit(
      [](const auto& variant) -> std::optional<double> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, double>) {
          return variant;
        }

        return std::nullopt;
      },
      value);
}

std::optional<std::string> Value::GetString() const {
  return std::visit(
      [](const auto& variant) -> std::optional<std::string> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, std::string>) {
          return variant;
        }

        return std::nullopt;
      },
      value);
}

std::string Value::ToString() const {
  return std::visit(
      [](const auto& variant) {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, int64_t>) {
          return std::to_string(variant);
        }

        if constexpr (std::is_same_v<T, double>) {
          return std::to_string(variant);
        }

        if constexpr (std::is_same_v<T, std::string>) {
          return "\"" + variant + "\"";
        }
      },
      value);
}

Value Value::FromString(std::string_view value, DataType data_type) {
  switch (data_type) {
    case DataType::kInteger:
      return Value{.value = std::stoll(value.data())};
    case DataType::kReal:
      return Value{.value = std::stod(value.data())};
    case DataType::kText:
      return Value{.value = std::string{value}};
    default:
      break;
  }

  return {};
}
}  // namespace stonks::db