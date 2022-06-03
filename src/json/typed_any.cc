#include "typed_any.h"

namespace stonks::json {
auto TypedAny::GetAny() const -> const json::Any& { return value_; }
}  // namespace stonks::json