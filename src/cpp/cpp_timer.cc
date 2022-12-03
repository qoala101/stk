#include "cpp_timer.h"

#include <absl/time/clock.h>
#include <absl/time/time.h>

#include <algorithm>
#include <stop_token>
#include <type_traits>
#include <utility>

namespace stonks::cpp {
Timer::Timer(fu2::unique_function<void() const> event, absl::Duration interval)
    : thread_{[event = std::move(event),
               interval](const std::stop_token &stop_token) {
        while (!stop_token.stop_requested()) {
          const auto next_event_time = absl::Now() + interval;

          event();

          while (!stop_token.stop_requested()) {
            const auto time_till_next_event = next_event_time - absl::Now();

            if (const auto no_time_left =
                    time_till_next_event < absl::ZeroDuration()) {
              break;
            }

            absl::SleepFor(std::min(time_till_next_event, absl::Seconds(1)));
          }
        }
      }} {}

Timer::~Timer() { thread_.request_stop(); }
}  // namespace stonks::cpp