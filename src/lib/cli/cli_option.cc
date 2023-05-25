/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cli_option.h"

#include <utility>

namespace vh::cli::detail {
OptionBase::OptionBase(cpp::NnSp<OptionValueVariant> value)
    : value_{std::move(value)} {}

auto OptionBase::GetValueImpl(cpp::This<OptionBase> auto& t) -> auto& {
  return *t.value_;
}

auto OptionBase::GetValue() const -> const OptionValueVariant& {
  return GetValueImpl(*this);
}

auto OptionBase::GetValue() -> OptionValueVariant& {
  return GetValueImpl(*this);
}
}  // namespace vh::cli::detail