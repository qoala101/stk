#include "network_json_common_conversions.h"

#include <cstdint>
#include <string>
#include <string_view>

#include "cpp_chrono.h"
#include "network_json_basic_conversions.h"
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

template <>
auto ParseFromJson(const IJson &json) -> cpp::TimePoint {
  return cpp::TimePoint{ParseFromJson<int64_t>(json)};
}

auto ConvertToJson(cpp::TimePoint value) -> cpp::Pv<IJson> {
  return ConvertToJson(value.count());
}
}  // namespace stonks::network