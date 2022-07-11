#include <cpprest/json.h>

#include "network_json_basic_conversions.h"
#include "restsdk_json.h"
#include "restsdk_json_impl.h"

namespace stonks::network {
template <>
auto ParseFromJson(const stonks::network::IJson &json) -> int {
  return json.GetImpl().GetJson().as_integer();
}

template <>
auto ParseFromJson(const stonks::network::IJson &json) -> int64_t {
  return json.GetImpl().GetJson().as_number().to_int64();
}

template <>
auto ParseFromJson(const stonks::network::IJson &json) -> double {
  return json.GetImpl().GetJson().as_double();
}

template <>
auto ParseFromJson(const stonks::network::IJson &json) -> std::string {
  return json.GetImpl().GetJson().as_string();
}

template <>
auto ConvertToJson(const int &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>> {
  return cpp::assume_not_null(
      std::make_unique<restsdk::Json>(IJson::Impl{value}));
}

template <>
auto ConvertToJson(const int64_t &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>> {
  return cpp::assume_not_null(
      std::make_unique<restsdk::Json>(IJson::Impl{value}));
}

template <>
auto ConvertToJson(const double &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>> {
  return cpp::assume_not_null(
      std::make_unique<restsdk::Json>(IJson::Impl{value}));
}

template <>
auto ConvertToJson(const std::string &value)
    -> cpp::not_null<std::unique_ptr<stonks::network::IJson>> {
  return cpp::assume_not_null(std::make_unique<restsdk::Json>(
      IJson::Impl{web::json::value::string(value)}));
}
}  // namespace stonks::network