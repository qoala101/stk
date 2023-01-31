#ifndef VH_CPP_TIMER_EVENT_LOOP_H_
#define VH_CPP_TIMER_EVENT_LOOP_H_

#include <absl/time/time.h>

#include <function2/function2.hpp>
#include <stop_token>

#include "cpp_timer_reattempt_policy.h"

namespace vh::cpp::timer {
/**
 * @brief Timer event loop.
 */
class EventLoop {
 public:
  /**
   * @param event_interval Minimum time between the end and start of successful
   * consecutive events.
   * @param reattempt_policy Tells how to reattempt the execution if it throws.
   */
  EventLoop(fu2::unique_function<void()> event, absl::Duration event_interval,
            ReattemptPolicy reattempt_policy);

  /**
   * @brief Starts the loop which can be interrupted via stop token.
   */
  void operator()(const std::stop_token& stop_token);

 private:
  void RunEventLoop();

  auto ExecuteEvent [[nodiscard]] ();

  void RunReattemptLoop();

  void InterruptableSleepFor(absl::Duration sleep_duration) const;

  fu2::unique_function<void()> event_{};
  absl::Duration event_interval_{};
  ReattemptPolicy reattempt_policy_{};
  const std::stop_token* stop_token_{};
};
}  // namespace vh::cpp::timer

#endif  // VH_CPP_TIMER_EVENT_LOOP_H_
