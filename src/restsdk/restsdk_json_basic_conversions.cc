#include <bits/exception.h>
#include <cpprest/json.h>
#include <polymorphic_value.h>

#include <cstdint>
#include <gsl/assert>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "network_i_json.h"
#include "network_json_basic_conversions.h"
#include "not_null.hpp"
#include "restsdk_json.h"
#include "restsdk_json_impl.h"

namespace stonks::network {
namespace {
class GenericException : public std::exception {
 public:
  explicit GenericException(std::string message)
      : message_{std::move(message)} {}

  [[nodiscard]] auto what() const noexcept -> const char * override {
    return message_.c_str();
  }

 private:
  std::string message_{};
};
}  // namespace

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

template <>
auto ParseFromJson(const IJson &json)
    -> cpp::not_null<std::unique_ptr<std::exception>> {
  const auto type = ParseFromJson<std::string>(*json.GetChild("restsdk:type"));

  if (type == "std::exception") {
    return cpp::assume_not_null(std::make_unique<GenericException>(
        ParseFromJson<std::string>(*json.GetChild("message"))));
  }

  Expects(false);
}

auto ConvertToJson(int value) -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{value});
}

auto ConvertToJson(int64_t value) -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{value});
}

auto ConvertToJson(double value) -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{value});
}

auto ConvertToJson(std::string_view value)
    -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{web::json::value::string(value.data())});
}

auto ConvertToJson(const std::exception &value)
    -> isocpp_p0201::polymorphic_value<IJson> {
  auto json = isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>();
  json->SetChild("restsdk:type",
                 restsdk::Json{network::IJson::Impl{
                     web::json::value::string("std::exception")}});
  json->SetChild("message", restsdk::Json{network::IJson::Impl{
                                web::json::value::string(value.what())}});
  return json;
}
}  // namespace stonks::network