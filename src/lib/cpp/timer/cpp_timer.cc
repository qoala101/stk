#include "cpp_timer.h"

#include <utility>

namespace stonks::cpp {
Timer::Timer(timer::EventLoop event_loop) : thread_{std::move(event_loop)} {}

Timer::~Timer() { thread_.request_stop(); }
}  // namespace stonks::cpp