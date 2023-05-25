/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "core_sdb_value_conversions.h"

#include <absl/time/time.h>

#include <cstdint>

#include "sqldb_value_conversions.h"

namespace vh::sqldb {
template <>
auto ValueParser<absl::Time>::operator()(const Value &value) const -> Type {
  return absl::FromUnixMillis(value.Get<int64_t>());
}

auto ConvertToValue(absl::Time time) -> Value {
  return Value{absl::ToUnixMillis(time)};
}
}  // namespace vh::sqldb