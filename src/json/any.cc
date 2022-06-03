#include "any.h"

namespace stonks::json {
auto Any::HasValue() const -> bool { return value_.has_value(); }
}  // namespace stonks::json