#ifndef STONKS_CPP_TIMER_CPP_TIMER_EVENT_LOOP_H_
#define STONKS_CPP_TIMER_CPP_TIMER_EVENT_LOOP_H_

#include <absl/time/time.h>

#include <function2/function2.hpp>
#include <stop_token>

#include "cpp_timer_reattempt_policy.h"

namespace stonks::cpp::timer {
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
  EventLoop(fu2::unique_function<void() const> event,
            absl::Duration event_interval, ReattemptPolicy reattempt_policy);

  /**
   * @brief Starts the loop which can be interrupted via stop token.
   */
  void operator()(const std::stop_token& stop_token);

 private:
  void RunEventLoop() const;

  auto ExecuteEvent [[nodiscard]] () const;

  void RunReattemptLoop() const;

  void InterruptableSleepFor(absl::Duration sleep_duration) const;

  fu2::unique_function<void() const> event_{};
  absl::Duration event_interval_{};
  ReattemptPolicy reattempt_policy_{};
  const std::stop_token* stop_token_{};
};
}  // namespace stonks::cpp::timer

#endif  // STONKS_CPP_TIMER_CPP_TIMER_EVENT_LOOP_H_
