#include "network_json_common_conversions.h"

#include <string>
#include <string_view>
#include <variant>

#include "network_json_base_conversions.h"
#include "network_json_conversions_facades.h"

namespace vh::network {
auto ConvertToJson(const char *value) -> cpp::Pv<IJson> {
  return ConvertToJson(std::string_view{value});
}

template <>
auto JsonParser<std::monostate>::operator()(const IJson &json) const -> Type {
  if (!json.IsNull()) {
    throw cpp::MessageException{"JSON is not null"};
  }

  return {};
}

auto ConvertToJson(std::monostate /*unused*/) -> cpp::Pv<IJson> {
  return CreateNullJson();
}

template <>
auto JsonParser<cpp::MessageException>::operator()(const IJson &json) const
    -> Type {
  return Type{ParseFromJsonChild<std::string>(json, "message")};
}

auto ConvertToJson(const std::exception &value) -> cpp::Pv<IJson> {
  return BuildJsonFrom("message", value.what());
}
}  // namespace vh::network