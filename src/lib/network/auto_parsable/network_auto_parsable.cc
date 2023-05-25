/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "network_auto_parsable.h"

#include <gsl/assert>
#include <utility>

namespace vh::network {
AutoParsable::AutoParsable(cpp::Pv<IJson> json) : json_{std::move(json)} {
  Ensures(json_.has_value());
}
}  // namespace vh::network