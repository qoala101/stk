#include "network_wrong_type_exception.h"

#include <utility>

namespace stonks::network {
WrongTypeException::WrongTypeException(std::string message, cpp::Pv<IJson> json)
    : Exception{std::move(message)}, json_{std::move(json)} {}

auto WrongTypeException::GetJsonImpl(cpp::DecaysTo<WrongTypeException> auto&& t)
    -> cpp::CopyConst<decltype(t), cpp::Pv<IJson>&> {
  return std::forward<decltype(t)>(t).json_;
}

auto WrongTypeException::GetJson() const -> const cpp::Pv<IJson>& {
  return GetJsonImpl(*this);
}

auto WrongTypeException::GetJson() -> cpp::Pv<IJson>& {
  return GetJsonImpl(*this);
}
}  // namespace stonks::network