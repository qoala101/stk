#include "typed_any.h"

namespace stonks::json {
auto TypedAny::GetAny() const -> const std::any& { return value_; }
}  // namespace stonks::json