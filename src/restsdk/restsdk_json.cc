#include "restsdk_json.h"

#include <cpprest/json.h>

#include <memory>
#include <utility>

#include "cpp_not_null.h"
#include "network_i_json.h"

namespace stonks::restsdk {
Json::Json(IJson::Impl impl) : impl_{std::move(impl)} {}

auto Json::clone() const -> cpp::NnUp<IJson> {
  return cpp::MakeNnUp<Json>(impl_);
}

auto Json::GetChild(std::string_view key) const& -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(
      IJson::Impl{impl_.GetJson().at(key.data())});
}

auto Json::GetChild(std::string_view key) && -> cpp::Pv<IJson> {
  return cpp::MakePv<IJson, restsdk::Json>(
      IJson::Impl{std::move(impl_.GetJson().at(key.data()))});
}

void Json::SetChild(std::string key, const IJson& child) {
  impl_.GetJson()[key] = child.GetImpl().GetJson();
}

void Json::SetChild(std::string key, IJson&& child) {
  impl_.GetJson()[key] = std::move(child.GetImpl().GetJson());
}

auto Json::GetImpl() const -> const IJson::Impl& { return impl_; }

auto Json::GetImpl() -> IJson::Impl& { return impl_; }
}  // namespace stonks::restsdk