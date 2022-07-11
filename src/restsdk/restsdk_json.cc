#include "restsdk_json.h"

#include <cpprest/json.h>

#include <memory>
#include <utility>

#include "network_json.h"
#include "not_null.hpp"

namespace stonks::restsdk {
Json::Json(IJson::Impl impl) : impl_{std::move(impl)} {}

auto Json::Clone() const -> cpp::not_null<std::unique_ptr<IJson>> {
  return cpp::assume_not_null(std::make_unique<Json>(impl_));
}

auto Json::GetChild(std::string_view key) const
    -> cpp::not_null<std::unique_ptr<IJson>> {
  return cpp::assume_not_null(
      std::make_unique<Json>(IJson::Impl{impl_.GetJson().at(key.data())}));
}

void Json::SetChild(std::string_view key, const IJson &child) {
  impl_.GetJson()[key.data()] = child.GetImpl().GetJson();
}

auto Json::GetImpl() const -> const IJson::Impl & { return impl_; }
}  // namespace stonks::restsdk

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