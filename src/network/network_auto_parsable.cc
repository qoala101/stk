#include "network_auto_parsable.h"

#include <utility>

namespace stonks::network {
AutoParsable::AutoParsable(isocpp_p0201::polymorphic_value<IJson> json)
    : json_{std::move(json)} {}
}  // namespace stonks::network