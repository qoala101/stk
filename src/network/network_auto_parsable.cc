#include "network_auto_parsable.h"

#include <utility>

namespace stonks::network {
AutoParsable::AutoParsable(cpp::Pv<IJson> json) : json_{std::move(json)} {}
}  // namespace stonks::network