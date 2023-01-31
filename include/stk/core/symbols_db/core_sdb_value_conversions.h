#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_SDB_VALUE_CONVERSIONS_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_SDB_VALUE_CONVERSIONS_H_

#include <absl/time/time.h>

#include "sqldb_value.h"

namespace vh::sqldb {
auto ConvertToValue [[nodiscard]] (absl::Time time) -> Value;
}  // namespace vh::sqldb

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_SDB_VALUE_CONVERSIONS_H_
