#ifndef VH_STK_CORE_SDB_VALUE_CONVERSIONS_H_
#define VH_STK_CORE_SDB_VALUE_CONVERSIONS_H_

#include <absl/time/time.h>

#include "sqldb_value.h"

namespace vh::sqldb {
auto ConvertToValue [[nodiscard]] (absl::Time time) -> Value;
}  // namespace vh::sqldb

#endif  // VH_STK_CORE_SDB_VALUE_CONVERSIONS_H_
