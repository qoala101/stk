#include <cpprest/json.h>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"

namespace stonks::network {
auto CreateNullJson() -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>();
}

template <>
auto ParseFromJson(const IJson &json) -> bool {
  return json.GetNativeHandle()->as_bool();
}

auto ConvertToJson(bool value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto ParseFromJson(const IJson &json) -> int {
  return json.GetNativeHandle()->as_integer();
}

auto ConvertToJson(int value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto ParseFromJson(const IJson &json) -> int64_t {
  return json.GetNativeHandle()->as_number().to_int64();
}

auto ConvertToJson(int64_t value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto ParseFromJson(const IJson &json) -> double {
  return json.GetNativeHandle()->as_double();
}

auto ConvertToJson(double value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto ParseFromJson(const IJson &json) -> std::string {
  return json.GetNativeHandle()->as_string();
}

auto ConvertToJson(std::string_view value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}
}  // namespace stonks::network