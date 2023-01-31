#include "restsdk_json_native_handle.h"

#include <cpprest/json.h>

#include <not_null.hpp>
#include <utility>

#include "cpp_not_null.h"
#include "network_i_json.h"

namespace stonks::network {
IJson::NativeHandle::NativeHandle()
    : json_{cpp::MakeNnUp<web::json::value>()} {}

IJson::NativeHandle::NativeHandle(const web::json::value &json)
    : json_{cpp::MakeNnUp<web::json::value>(json)} {}

IJson::NativeHandle::NativeHandle(web::json::value &&json)
    : json_{cpp::MakeNnUp<web::json::value>(std::move(json))} {}

IJson::NativeHandle::NativeHandle(const NativeHandle &other)
    : json_{cpp::MakeNnUp<web::json::value>(*other.json_)} {}

IJson::NativeHandle::NativeHandle(NativeHandle &&) noexcept = default;

auto IJson::NativeHandle::operator=(const NativeHandle &other)
    -> NativeHandle & {
  if (&other == this) {
    return *this;
  }

  json_ = cpp::MakeNnUp<web::json::value>(*other.json_);

  return *this;
}

auto IJson::NativeHandle::operator=(NativeHandle &&) noexcept
    -> NativeHandle & = default;

IJson::NativeHandle::~NativeHandle() = default;

auto IJson::NativeHandle::OperatorAsteriskImpl(
    cpp::This<IJson::NativeHandle> auto &t) -> auto & {
  return *t.json_;
}

auto IJson::NativeHandle::operator*() const -> const web::json::value & {
  return OperatorAsteriskImpl(*this);
}

auto IJson::NativeHandle::operator*() -> web::json::value & {
  return OperatorAsteriskImpl(*this);
}

auto IJson::NativeHandle::OperatorArrowImpl(cpp::This<NativeHandle> auto &t) {
  return cpp::AssumeNn(&*t);
}

auto IJson::NativeHandle::operator->() const
    -> cpp::Nn<const web::json::value *> {
  return OperatorArrowImpl(*this);
}

auto IJson::NativeHandle::operator->() -> cpp::Nn<web::json::value *> {
  return OperatorArrowImpl(*this);
}
}  // namespace stonks::network