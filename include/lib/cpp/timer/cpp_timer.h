/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_CPP_TIMER_H_
#define VH_CPP_TIMER_H_

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

#endif  // VH_CPP_TIMER_H_
