#include "type_variant.h"

namespace stonks::json {
auto TypeVariant::ParseAnyFromJson(const web::json::value &json) const
    -> std::any {
  return std::visit(
      [&json](const auto &variant) { return variant.ParseAnyFromJson(json); },
      type_);
}

auto TypeVariant::ConvertAnyToJson(const std::any &data) const
    -> std::optional<web::json::value> {
  return std::visit(
      [&data](const auto &variant) { return variant.ConvertAnyToJson(data); },
      type_);
}

auto TypeVariant::MakeNulloptAny() const -> std::any {
  return std::visit(
      [](const auto &variant) { return variant.MakeNulloptAny(); }, type_);
}
}  // namespace stonks::json