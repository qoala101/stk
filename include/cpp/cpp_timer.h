#ifndef STONKS_CPP_CPP_TIMER_H_
#define STONKS_CPP_CPP_TIMER_H_

#include <absl/time/time.h>

#include <function2/function2.hpp>
#include <thread>

namespace stonks::cpp {
/**
 * @brief Info about how to reattempt execution of failed event.
 */
struct ReattemptPolicy {
  absl::Duration reattempt_interval{};
  int num_reattempts{};
};

/**
 * @brief Executes event at specified interval in the separate thread.
 */
class Timer {
 public:
  /**
   * @brief Starts the thread which executes the event.
   * @param interval Minimum time between the end and start of successful
   * consecutive events.
   * @param reattempt Tells how to reattempt the execution if it throws.
   */
  Timer(fu2::unique_function<void() const> event, absl::Duration event_interval,
        const ReattemptPolicy &reattempt_policy = {});

  Timer(const Timer &) = delete;
  Timer(Timer &&) noexcept = default;

  auto operator=(const Timer &) -> Timer & = delete;
  auto operator=(Timer &&) noexcept -> Timer & = default;

  /**
   * @brief Stops the thread and execution.
   */
  ~Timer();

 private:
  std::jthread thread_{};
};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TIMER_H_
