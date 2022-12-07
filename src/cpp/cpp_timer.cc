#include "cpp_timer.h"

#include <absl/time/clock.h>
#include <absl/time/time.h>

#include <algorithm>
#include <gsl/assert>
#include <stop_token>
#include <utility>

namespace stonks::cpp {
namespace {
class TimerImpl {
 public:
  TimerImpl(fu2::unique_function<void() const> event,
            absl::Duration event_interval, ReattemptPolicy reattempt_policy)
      : event_{[&event]() {
          Expects(!event.empty());
          return std::move(event);
        }()},
        event_interval_{event_interval},
        reattempt_policy_{reattempt_policy} {
    Ensures(!event_.empty());
  }

  void operator()(const std::stop_token &stop_token) {
    Expects(stop_token_ == nullptr);
    stop_token_ = &stop_token;
    Ensures(stop_token_ != nullptr);

    RunEventLoop();
  }

 private:
  void RunEventLoop() const {
    Expects(stop_token_ != nullptr);

    while (!stop_token_->stop_requested()) {
      RunReattemptLoop();
      InterruptableSleepFor(event_interval_);
    }
  }

  auto ExecuteEvent [[nodiscard]] () const {
    auto event_succeeded = true;

    try {
      event_();
    } catch (...) {
      event_succeeded = false;
    }

    return event_succeeded;
  }

  void RunReattemptLoop() const {
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

  void InterruptableSleepFor(absl::Duration sleep_duration) const {
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

  fu2::unique_function<void() const> event_{};
  absl::Duration event_interval_{};
  ReattemptPolicy reattempt_policy_{};
  const std::stop_token *stop_token_{};
};
}  // namespace

Timer::Timer(fu2::unique_function<void() const> event,
             absl::Duration event_interval,
             const ReattemptPolicy &reattempt_policy)
    : thread_{TimerImpl{std::move(event), event_interval, reattempt_policy}} {}

Timer::~Timer() { thread_.request_stop(); }
}  // namespace stonks::cpp