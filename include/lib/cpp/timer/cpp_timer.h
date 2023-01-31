#ifndef STONKS_CPP_TIMER_CPP_TIMER_H_
#define STONKS_CPP_TIMER_CPP_TIMER_H_

#include <thread>

#include "cpp_timer_event_loop.h"

namespace vh::cpp {
/**
 * @brief Executes event at specified interval in the separate thread.
 */
class Timer {
 public:
  /**
   * @brief Starts the thread which executes the event loop.
   */
  explicit Timer(timer::EventLoop event_loop);

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
}  // namespace vh::cpp

#endif  // STONKS_CPP_TIMER_CPP_TIMER_H_
