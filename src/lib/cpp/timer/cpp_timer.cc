/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cpp_timer.h"

#include <utility>

namespace vh::cpp {
Timer::Timer(timer::EventLoop event_loop) : thread_{std::move(event_loop)} {}

Timer::~Timer() { thread_.request_stop(); }
}  // namespace vh::cpp