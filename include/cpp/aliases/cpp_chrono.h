#ifndef STONKS_CPP_ALIASES_CPP_CHRONO_H_
#define STONKS_CPP_ALIASES_CPP_CHRONO_H_

#include <chrono>

/**
 * @file Short std::chrono aliases.
 */

namespace stonks::cpp {
/**
 * @brief Used for time measurement.
 */
using TimePoint = std::chrono::milliseconds;

/**
 * @brief Gives current time point.
 */
[[nodiscard]] auto GetCurrentTime() -> TimePoint;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_ALIASES_CPP_CHRONO_H_
