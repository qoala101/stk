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

auto StartTimeFrom(const absl::Time *time) -> absl::Time {
  return (time == nullptr) ? absl::InfinitePast() : *time;
}

auto EndTimeFrom(const absl::Time *time) -> absl::Time {
  return (time == nullptr) ? absl::InfiniteFuture() : *time;
}

auto TimeOrderFrom(const TimeOrder *order) -> TimeOrder {
  return (order == nullptr) ? TimeOrder::kOldFirst : *order;
}
}  // namespace stonks::core
