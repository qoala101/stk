#ifndef STONKS_APP_CORE_CORE_COMMON_H_
#define STONKS_APP_CORE_CORE_COMMON_H_

#include <absl/time/time.h>

#include <cmath>

#include "core_types.h"

namespace vh::stk::core {
struct CeilFloorArgs {
  double value{};
  double precision{1};
};

auto Ceil [[nodiscard]] (const CeilFloorArgs &args) -> double;

auto Floor [[nodiscard]] (const CeilFloorArgs &args) -> double;

auto StartTimeFrom [[nodiscard]] (const cpp::Opt<absl::Time> &time) -> absl::Time;

auto EndTimeFrom [[nodiscard]] (const cpp::Opt<absl::Time> &time) -> absl::Time;

auto TimeOrderFrom [[nodiscard]] (const cpp::Opt<TimeOrder> &order) -> TimeOrder;
}  // namespace vh::stk::core

#endif  // STONKS_APP_CORE_CORE_COMMON_H_
