#include "restsdk_json.h"

#include <cpprest/json.h>

#include <memory>
#include <utility>

#include "network_i_json.h"
#include "not_null.hpp"

namespace stonks::restsdk {
Json::Json(IJson::Impl impl) : impl_{std::move(impl)} {}

auto Json::clone() const -> cpp::not_null<std::unique_ptr<IJson>> {
  return cpp::assume_not_null(std::make_unique<Json>(impl_));
}

auto Json::GetChild(
    std::string_view key) const& -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{impl_.GetJson().at(key.data())});
}

auto Json::GetChild(
    std::string_view key) && -> isocpp_p0201::polymorphic_value<IJson> {
  return isocpp_p0201::make_polymorphic_value<IJson, restsdk::Json>(
      IJson::Impl{std::move(impl_.GetJson().at(key.data()))});
}

void Json::SetChild(std::string_view key, const IJson& child) {
  impl_.GetJson()[key.data()] = child.GetImpl().GetJson();
}

void Json::SetChild(std::string_view key, IJson&& child) {
  impl_.GetJson()[key.data()] = std::move(child.GetImpl().GetJson());
}

auto Json::GetImpl() const -> const IJson::Impl& { return impl_; }

auto Json::GetImpl() -> IJson::Impl& { return impl_; }
}  // namespace stonks::restsdk