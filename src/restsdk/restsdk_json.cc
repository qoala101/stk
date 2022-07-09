#include "restsdk_json.h"

#include <utility>

namespace stonks::restsdk {
Json::Json(network::IJson::Impl impl) : impl_{std::move(impl)} {}

auto Json::Clone() const -> cpp::not_null<std::unique_ptr<IJson>> {
  return cpp::assume_not_null(std::make_unique<Json>(impl_));
}

auto Json::GetImpl() const -> const network::IJson::Impl& { return impl_; }
}  // namespace stonks::restsdk