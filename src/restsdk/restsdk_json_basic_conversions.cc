#include <cpprest/json.h>
#include <polymorphic_value.h>
#include <stdint.h>

#include <memory>
#include <string>

#include "network_json.h"
#include "network_json_basic_conversions.h"
#include "restsdk_json.h"
#include "restsdk_json_impl.h"

namespace stonks::network {
template <>
auto ParseFromJson(const IJson &json) -> int {
  return json.GetImpl().GetJson().as_integer();
}

template <>
auto ParseFromJson(const IJson &json) -> int64_t {
  return json.GetImpl().GetJson().as_number().to_int64();
}

template <>
auto ParseFromJson(const IJson &json) -> double {
  return json.GetImpl().GetJson().as_double();
}

template <>
auto ParseFromJson(const IJson &json) -> std::string {
  return json.GetImpl().GetJson().as_string();
}

auto ConvertToJson(int value) -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{value});
}

auto ConvertToJson(int64_t value)
    -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{value});
}

auto ConvertToJson(double value)
    -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{value});
}

auto ConvertToJson(std::string_view value)
    -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{web::json::value::string(value.data())});
}
}  // namespace stonks::network