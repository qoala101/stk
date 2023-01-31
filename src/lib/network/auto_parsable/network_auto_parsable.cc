#include "network_auto_parsable.h"

#include <gsl/assert>
#include <utility>

namespace stonks::network {
AutoParsable::AutoParsable(cpp::Pv<IJson> json) : json_{std::move(json)} {
  Ensures(json_.has_value());
}
}  // namespace stonks::network