/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "core_common.h"

#include <cmath>
#include <gsl/assert>

namespace vh::stk::core {
auto Ceil(const CeilFloorArgs &args) -> double {
  Expects(args.precision != 0);
  return std::ceil(args.value / args.precision) * args.precision;
}

auto Floor(const CeilFloorArgs &args) -> double {
  Expects(args.precision != 0);
  return std::floor(args.value / args.precision) * args.precision;
}

auto StartTimeFrom(const cpp::Opt<absl::Time> &time) -> absl::Time {
  return time.value_or(absl::InfinitePast());
}

auto EndTimeFrom(const cpp::Opt<absl::Time> &time) -> absl::Time {
  return time.value_or(absl::InfiniteFuture());
}

auto TimeOrderFrom(const cpp::Opt<TimeOrder> &order) -> TimeOrder {
  return order.value_or(TimeOrder::kOldFirst);
}
}  // namespace vh::stk::core
