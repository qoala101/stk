#include "network_json_common_conversions.h"

#include <string>
#include <string_view>

#include "network_json_conversions_facades.h"

namespace stonks::network {
auto ConvertToJson(const char *value) -> cpp::Pv<IJson> {
  return ConvertToJson(std::string_view{value});
}

template <>
auto ParseFromJson(const IJson &json) -> cpp::MessageException {
  return cpp::MessageException{
      ParseFromJsonChild<std::string>(json, "message")};
}

auto ConvertToJson(const std::exception &value) -> cpp::Pv<IJson> {
  return BuildJsonFrom("message", value.what());
}
}  // namespace stonks::network