#include "any.h"

namespace stonks::any {
auto Any::HasValue() const -> bool { return value_.has_value(); }
}  // namespace stonks::any