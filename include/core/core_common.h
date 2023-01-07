#ifndef STONKS_CORE_CORE_COMMON_H_
#define STONKS_CORE_CORE_COMMON_H_

#include <absl/time/time.h>

#include <cmath>

#include "core_types.h"

namespace stonks::core {
namespace detail {
struct CeilFloorArgs {
  double value{};
  double precision{1};
};
}  // namespace detail

auto Ceil [[nodiscard]] (const detail::CeilFloorArgs &args) -> double;

auto Floor [[nodiscard]] (const detail::CeilFloorArgs &args) -> double;

auto StartTimeFrom [[nodiscard]] (const absl::Time *time) -> absl::Time;

auto EndTimeFrom [[nodiscard]] (const absl::Time *time) -> absl::Time;

auto TimeOrderFrom [[nodiscard]] (const TimeOrder *order) -> TimeOrder;
}  // namespace stonks::core

#endif  // STONKS_CORE_CORE_COMMON_H_
