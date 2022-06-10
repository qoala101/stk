#include "typed_any.h"

namespace stonks::json {
auto TypedAny::GetAny() const -> const any::Any& { return value_; }
}  // namespace stonks::json