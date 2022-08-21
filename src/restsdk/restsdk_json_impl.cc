#include "restsdk_json_impl.h"

#include <cpprest/json.h>

#include <utility>

#include "ccutils_not_null.h"
#include "network_i_json.h"
#include "not_null.hpp"

namespace stonks::network {
IJson::Impl::Impl() : json_{ccutils::MakeNnUp<web::json::value>()} {}

IJson::Impl::Impl(const web::json::value &json)
    : json_{ccutils::MakeNnUp<web::json::value>(json)} {}

IJson::Impl::Impl(web::json::value &&json)
    : json_{ccutils::MakeNnUp<web::json::value>(std::move(json))} {}

IJson::Impl::Impl(const Impl &other)
    : json_{ccutils::MakeNnUp<web::json::value>(*other.json_)} {}

IJson::Impl::Impl(Impl &&) noexcept = default;

auto IJson::Impl::operator=(const Impl &other) -> Impl & {
  if (&other == this) {
    return *this;
  }

  json_ = ccutils::MakeNnUp<web::json::value>(*other.json_);

  return *this;
}

auto IJson::Impl::operator=(Impl &&) noexcept -> Impl & = default;

IJson::Impl::~Impl() = default;

auto IJson::Impl::GetJson() const -> const web::json::value & { return *json_; }

auto IJson::Impl::GetJson() -> web::json::value & { return *json_; }
}  // namespace stonks::network