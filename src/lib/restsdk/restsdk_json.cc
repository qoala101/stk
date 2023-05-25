/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "restsdk_json.h"

#include <cpprest/json.h>

#include <gsl/assert>
#include <gsl/util>
#include <memory>
#include <not_null.hpp>
#include <utility>

#include "cpp_move_if.h"
#include "cpp_not_null.h"
#include "network_i_json.h"

namespace vh::restsdk {
Json::Json(bool value) : native_handle_{web::json::value::boolean(value)} {}

Json::Json(int value) : native_handle_{web::json::value::number(value)} {}

Json::Json(int64_t value) : native_handle_{web::json::value::number(value)} {}

Json::Json(double value) : native_handle_{web::json::value::number(value)} {}

Json::Json(std::string_view value)
    : native_handle_{web::json::value::string(value.data())} {}

Json::Json(IJson::NativeHandle impl) : native_handle_{std::move(impl)} {}

template <cpp::This<Json> This>
auto Json::CloneImpl(This& t) {
  return cpp::MakeNnUp<Json>(cpp::MoveIfNotConst<This>(t.native_handle_));
}

auto Json::clone() const -> cpp::NnUp<IJson> { return CloneImpl(*this); }

auto Json::clone() -> cpp::NnUp<IJson> { return CloneImpl(*this); }

auto Json::IsNull() const -> bool { return native_handle_->is_null(); }

template <cpp::This<Json> This>
auto Json::GetChildImpl(This& t, std::string_view key) {
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

template <cpp::This<Json> This>
auto Json::GetChildImpl(This& t, int index) {
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
  if (native_handle_->is_null()) {
    return 0;
  }

  return gsl::narrow_cast<int>(native_handle_->as_array().size());
}

auto Json::GetNativeHandleImpl(cpp::This<Json> auto& t) -> auto& {
  return t.native_handle_;
}

auto Json::GetNativeHandle() const -> const IJson::NativeHandle& {
  return GetNativeHandleImpl(*this);
}

auto Json::GetNativeHandle() -> IJson::NativeHandle& {
  return GetNativeHandleImpl(*this);
}
}  // namespace vh::restsdk