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
Json::Json(bool value) : native_handle_{web::json::value::boolean(value)} {}

Json::Json(int value) : native_handle_{web::json::value::number(value)} {}

Json::Json(int64_t value) : native_handle_{web::json::value::number(value)} {}

Json::Json(double value) : native_handle_{web::json::value::number(value)} {}

Json::Json(const char* value)
    : native_handle_{web::json::value::string(value)} {}

Json::Json(std::string_view value)
    : native_handle_{web::json::value::string(value.data())} {}

Json::Json(IJson::NativeHandle impl) : native_handle_{std::move(impl)} {}

template <cpp::DecaysTo<Json> This>
auto Json::CloneImpl(This& t) -> cpp::NnUp<IJson> {
  return cpp::MakeNnUp<Json>(cpp::MoveIfNotConst<This>(t.native_handle_));
}

auto Json::clone() const -> cpp::NnUp<IJson> { return CloneImpl(*this); }

auto Json::clone() -> cpp::NnUp<IJson> { return CloneImpl(*this); }

auto Json::IsNull() const -> bool { return native_handle_->is_null(); }

template <cpp::DecaysTo<Json> This>
auto Json::GetChildImpl(This& t, std::string_view key) -> cpp::Pv<IJson> {
  Expects(!t.native_handle_->is_array());
  return cpp::MakePv<IJson, restsdk::Json>(IJson::NativeHandle{
      cpp::MoveIfNotConst<This>(t.native_handle_->at(key.data()))});
}

auto Json::GetChild(std::string_view key) const -> cpp::Pv<IJson> {
  return GetChildImpl(*this, key);
}

auto Json::GetChild(std::string_view key) -> cpp::Pv<IJson> {
  return GetChildImpl(*this, key);
}

void Json::SetChild(std::string key, cpp::Pv<IJson> child) {
  Expects(!native_handle_->is_array());
  (*native_handle_)[key] = std::move(*child->GetNativeHandle());
}

template <cpp::DecaysTo<Json> This>
auto Json::GetChildImpl(This& t, int index) -> cpp::Pv<IJson> {
  Expects(t.native_handle_->is_array());
  return cpp::MakePv<IJson, restsdk::Json>(IJson::NativeHandle{
      cpp::MoveIfNotConst<This>(t.native_handle_->as_array().at(index))});
}

auto Json::GetChild(int index) const -> cpp::Pv<IJson> {
  return GetChildImpl(*this, index);
}

auto Json::GetChild(int index) -> cpp::Pv<IJson> {
  return GetChildImpl(*this, index);
}

void Json::SetChild(int index, cpp::Pv<IJson> child) {
  Expects(native_handle_->is_array() || native_handle_->is_null());

  if (native_handle_->is_null()) {
    *native_handle_ = web::json::value::array();
  }

  native_handle_->as_array()[index] = std::move(*child->GetNativeHandle());

  Ensures(native_handle_->is_array());
  Ensures(GetSize() > 0);
}

auto Json::GetSize() const -> int {
  Expects(native_handle_->is_array() || native_handle_->is_null());

  if (native_handle_->is_null()) {
    return 0;
  }

  return gsl::narrow_cast<int>(native_handle_->as_array().size());
}

template <cpp::DecaysTo<Json> This>
auto Json::GetNativeHandleImpl(This& t)
    -> cpp::CopyConst<This, IJson::NativeHandle&> {
  return t.native_handle_;
}

auto Json::GetNativeHandle() const -> const IJson::NativeHandle& {
  return GetNativeHandleImpl(*this);
}

auto Json::GetNativeHandle() -> IJson::NativeHandle& {
  return GetNativeHandleImpl(*this);
}
}  // namespace stonks::restsdk