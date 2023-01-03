#ifndef STONKS_CPP_TIMER_CPP_TIMER_BUILDER_H_
#define STONKS_CPP_TIMER_CPP_TIMER_BUILDER_H_

#include <absl/time/time.h>

#include <function2/function2.hpp>

#include "cpp_optional.h"
#include "cpp_timer.h"

namespace stonks::cpp {
/**
 * @brief Convenience API to setup timed event.
 */
class TimerBuilder {
 public:
  /**
   * @param event Event to be executed on timer tick.
   */
  explicit TimerBuilder(fu2::unique_function<void()> event);

  /**
   * @brief Sets event interval.
   */
  auto Every(absl::Duration event_interval) -> TimerBuilder &;

  /**
   * @brief Would execute event only once.
   */
  auto Once() -> TimerBuilder &;

  /**
   * @brief Makes event to be reattempted immediately when failed.
   */
  auto IfThrowsReattempt() -> TimerBuilder &;

  /**
   * @brief Sets reattempt interval.
   */
  auto IfThrowsReattemptEvery(absl::Duration reattempt_interval)
      -> TimerBuilder &;

  /**
   * @brief Sets number of reattempts per iteration.
   */
  auto Times(int num_reattempts) -> TimerBuilder &;

  /**
   * @brief Starts the timer.
   */
  auto Start [[nodiscard]] () -> Timer;

 private:
  fu2::unique_function<void()> event_{};
  cpp::Opt<absl::Duration> event_interval_{};
  cpp::Opt<absl::Duration> reattempt_interval_{};
  cpp::Opt<int> num_reattempts_{};
};

/**
 * @brief Begins timer builder.
 */
auto Execute [[nodiscard]] (fu2::unique_function<void()> event) -> TimerBuilder;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_TIMER_CPP_TIMER_BUILDER_H_
