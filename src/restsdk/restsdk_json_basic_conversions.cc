#include <cpprest/json.h>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "cpp_polymorphic_value.h"
#include "network_i_json.h"
#include "network_json_base_conversions.h"
#include "not_null.hpp"
#include "restsdk_json.h"
#include "restsdk_json_native_handle.h"

namespace stonks::network {
auto CreateNullJson() -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>();
}

template <>
auto JsonParser<bool>::operator()(const IJson &json) const -> Type {
  return json.GetNativeHandle()->as_bool();
}

auto ConvertToJson(bool value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto JsonParser<int>::operator()(const IJson &json) const -> Type {
  return json.GetNativeHandle()->as_integer();
}

auto ConvertToJson(int value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto JsonParser<int64_t>::operator()(const IJson &json) const -> Type {
  return json.GetNativeHandle()->as_number().to_int64();
}

auto ConvertToJson(int64_t value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto JsonParser<double>::operator()(const IJson &json) const -> Type {
  return json.GetNativeHandle()->as_double();
}

auto ConvertToJson(double value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}

template <>
auto JsonParser<std::string>::operator()(const IJson &json) const -> Type {
  return json.GetNativeHandle()->as_string();
}

auto ConvertToJson(std::string_view value) -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(value);
}
}  // namespace stonks::network