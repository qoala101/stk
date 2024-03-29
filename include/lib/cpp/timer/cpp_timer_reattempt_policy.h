/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_TIMER_REATTEMPT_POLICY_H_
#define VH_CPP_TIMER_REATTEMPT_POLICY_H_

#include <absl/time/time.h>

namespace vh::cpp::timer {
/**
 * @brief Info about how to reattempt execution of failed event.
 */
struct ReattemptPolicy {
  absl::Duration reattempt_interval{};
  int num_reattempts{};
};
}  // namespace vh::cpp::timer

#endif  // VH_CPP_TIMER_REATTEMPT_POLICY_H_
