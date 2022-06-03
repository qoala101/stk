#include "type_variant.h"

#include <bits/exception.h>

#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/view.hpp>

namespace stonks::json {
auto TypeVariant::ParseAnyFromJson(const web::json::value &json) const
    -> json::Any {
  return std::visit(
      [&json](const auto &variant) { return variant.ParseAnyFromJson(json); },
      type_);
}

auto TypeVariant::ConvertAnyToJson(const json::Any &data) const
    -> std::optional<web::json::value> {
  return std::visit(
      [&data](const auto &variant) { return variant.ConvertAnyToJson(data); },
      type_);
}

auto TypeVariant::MakeNulloptAny() const -> json::Any {
  return std::visit(
      [](const auto &variant) { return variant.MakeNulloptAny(); }, type_);
}
}  // namespace stonks::json