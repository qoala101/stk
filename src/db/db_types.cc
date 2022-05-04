#include "db_types.h"

namespace stonks::db {
std::optional<int64_t> Value::GetInt64() const { return {}; }

std::optional<double> Value::GetDouble() const { return {}; }

std::optional<std::string> Value::GetString() const { return {}; }
}  // namespace stonks::db