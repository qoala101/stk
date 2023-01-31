#include "core_sdb_value_conversions.h"

#include <absl/time/time.h>

#include <cstdint>

#include "sqldb_value_conversions.h"

namespace stonks::sqldb {
template <>
auto ValueParser<absl::Time>::operator()(const Value &value) const -> Type {
  return absl::FromUnixMillis(value.Get<int64_t>());
}

auto ConvertToValue(absl::Time time) -> Value {
  return Value{absl::ToUnixMillis(time)};
}
}  // namespace stonks::sqldb