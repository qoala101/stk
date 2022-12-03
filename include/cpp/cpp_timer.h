#ifndef STONKS_CPP_CPP_TIMER_H_
#define STONKS_CPP_CPP_TIMER_H_

#include <absl/time/time.h>

#include <function2/function2.hpp>
#include <thread>

namespace stonks::cpp {
/**
 * @brief Executes event at specified interval in the separate thread.
 */
class Timer {
 public:
  /**
   * @brief Starts the thread which executes event.
   * @param interval Minimum time between the starts of consecutive events.
   */
  Timer(fu2::unique_function<void() const> event, absl::Duration interval);

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
