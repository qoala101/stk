/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cpp_timer_event_loop.h"

#include <absl/time/clock.h>
#include <absl/time/time.h>

#include <algorithm>
#include <gsl/assert>
#include <utility>

namespace vh::cpp::timer {
EventLoop::EventLoop(fu2::unique_function<void()> event,
                     absl::Duration event_interval,
                     ReattemptPolicy reattempt_policy)
    : event_{(Expects(!event.empty()), std::move(event))},
      event_interval_{event_interval},
      reattempt_policy_{reattempt_policy} {
  Ensures(!event_.empty());
}

void EventLoop::operator()(const std::stop_token &stop_token) {
  Expects(stop_token_ == nullptr);
  stop_token_ = &stop_token;

  RunEventLoop();

  stop_token_ = nullptr;
}

void EventLoop::RunEventLoop() {
  Expects(stop_token_ != nullptr);

  while (!stop_token_->stop_requested()) {
    RunReattemptLoop();

    if (const auto single_time_event =
            event_interval_ == absl::InfiniteDuration()) {
      break;
    }

    InterruptableSleepFor(event_interval_);
  }
}

auto EventLoop::ExecuteEvent [[nodiscard]] () {
  auto event_succeeded = true;

  try {
    event_();
  } catch (...) {
    event_succeeded = false;
  }

  return event_succeeded;
}

void EventLoop::RunReattemptLoop() {
  Expects(stop_token_ != nullptr);

  auto num_attempts = 0;

  while (!stop_token_->stop_requested()) {
    const auto event_succeeded = ExecuteEvent();

    if (event_succeeded) {
      break;
    }

    ++num_attempts;

    if (const auto reattempt_limit_reached =
            num_attempts > reattempt_policy_.num_reattempts) {
      break;
    }

    InterruptableSleepFor(reattempt_policy_.reattempt_interval);
  }
}

void EventLoop::InterruptableSleepFor(absl::Duration sleep_duration) const {
  Expects(stop_token_ != nullptr);

  const auto wake_up_time = absl::Now() + sleep_duration;

  while (!stop_token_->stop_requested()) {
    const auto time_till_wake_up = wake_up_time - absl::Now();

    if (const auto no_time_left = time_till_wake_up < absl::ZeroDuration()) {
      break;
    }

    absl::SleepFor(std::min(time_till_wake_up, absl::Seconds(1)));
  }
}
}  // namespace vh::cpp::timer
