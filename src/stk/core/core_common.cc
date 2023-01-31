#include "core_common.h"

#include <gsl/assert>

namespace stonks::core {
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
}  // namespace stonks::core
