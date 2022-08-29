#include "restsdk_json.h"

#include <cpprest/json.h>

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <utility>

#include "cpp_move_if.h"
#include "cpp_not_null.h"
#include "network_i_json.h"

namespace stonks::restsdk {
Json::Json(bool value) : impl_{web::json::value{value}} {}

Json::Json(int value) : impl_{web::json::value{value}} {}

Json::Json(int64_t value) : impl_{web::json::value{value}} {}

Json::Json(double value) : impl_{web::json::value{value}} {}

Json::Json(const char* value) : impl_{web::json::value::string(value)} {}

Json::Json(std::string_view value) : impl_{web::json::value::string(value.data())} {}

Json::Json(IJson::Impl impl) : impl_{std::move(impl)} {}

auto Json::CloneImpl(auto&& t) -> cpp::NnUp<IJson> {
  return cpp::MakeNnUp<Json>(cpp::MoveIfNotConst<decltype(t)>(t.impl_));
}

auto Json::clone() const& -> cpp::NnUp<IJson> { return CloneImpl(*this); }

auto Json::clone() && -> cpp::NnUp<IJson> { return CloneImpl(*this); }

auto Json::IsNull() const -> bool { return impl_.GetJson().is_null(); }

auto Json::GetChildImpl(auto&& t, std::string_view key) -> cpp::Pv<IJson> {
  auto& json = t.impl_.GetJson();
  Expects(!json.is_array());

  return cpp::MakePv<IJson, restsdk::Json>(
      IJson::Impl{cpp::MoveIfNotConst<decltype(t)>(json.at(key.data()))});
}

auto Json::GetChild(std::string_view key) const& -> cpp::Pv<IJson> {
  return GetChildImpl(*this, key);
}

auto Json::GetChild(std::string_view key) && -> cpp::Pv<IJson> {
  return GetChildImpl(*this, key);
}

void Json::SetChild(std::string key, cpp::Pv<IJson> child) {
  auto& json = impl_.GetJson();
  Expects(!json.is_array());

  json[key] = std::move(child->GetImpl().GetJson());
}

auto Json::GetChildImpl(auto&& t, int index) -> cpp::Pv<IJson> {
  auto& json = t.impl_.GetJson();
  Expects(json.is_array());

  return cpp::MakePv<IJson, restsdk::Json>(
      IJson::Impl{cpp::MoveIfNotConst<decltype(t)>(json.as_array().at(index))});
}

auto Json::GetChild(int index) const& -> cpp::Pv<IJson> {
  return GetChildImpl(*this, index);
}

auto Json::GetChild(int index) && -> cpp::Pv<IJson> {
  return GetChildImpl(*this, index);
}

void Json::SetChild(int index, cpp::Pv<IJson> child) {
  auto& json = impl_.GetJson();
  Expects(json.is_array() || json.is_null());

  if (json.is_null()) {
    json = web::json::value::array();
  }

  json.as_array()[index] = std::move(child->GetImpl().GetJson());

  Ensures(json.is_array());
  Ensures(GetSize() > 0);
}

auto Json::GetSize() const -> int {
  const auto& json = impl_.GetJson();
  Expects(json.is_array() || json.is_null());

  if (json.is_null()) {
    return 0;
  }

  return gsl::narrow_cast<int>(json.as_array().size());
}

auto Json::GetImpl() const -> const IJson::Impl& { return impl_; }

auto Json::GetImpl() -> IJson::Impl& { return impl_; }
}  // namespace stonks::restsdk