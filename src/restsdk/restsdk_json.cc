#include "restsdk_json.h"

#include <cpprest/json.h>

#include <cstdint>
#include <memory>
#include <utility>

#include "network_json.h"
#include "not_null.hpp"

namespace stonks::restsdk {
Json::Json(IJson::Impl impl) : impl_{std::move(impl)} {}

auto Json::Clone() const -> cpp::not_null<std::unique_ptr<IJson>> {
  return cpp::assume_not_null(std::make_unique<Json>(impl_));
}

auto Json::AsInt() const -> int { return impl_.GetJson().as_integer(); }

void Json::SetInt(int value) { impl_.GetJson() = value; }

auto Json::AsInt64() const -> int64_t {
  return impl_.GetJson().as_number().to_int64();
}

void Json::SetInt64(int64_t value) { impl_.GetJson() = value; }

auto Json::AsDouble() const -> double { return impl_.GetJson().as_double(); }

void Json::SetDouble(double value) { impl_.GetJson() = value; }

auto Json::AsString() const -> std::string {
  return impl_.GetJson().as_string();
}

void Json::SetString(std::string_view value) {
  impl_.GetJson() = web::json::value::string(std::string{value.data()});
}

auto Json::GetChild(std::string_view key) const
    -> cpp::not_null<std::unique_ptr<IJson>> {
  return cpp::assume_not_null(
      std::make_unique<Json>(IJson::Impl{impl_.GetJson().at(key.data())}));
}

void Json::SetChild(std::string_view key, const IJson& child) {
  impl_.GetJson()[key.data()] = child.GetImpl().GetJson();
}

auto Json::GetImpl() const -> const IJson::Impl& { return impl_; }
}  // namespace stonks::restsdk