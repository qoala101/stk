#include "cpp_timer_builder.h"

#include <absl/time/time.h>

#include <gsl/assert>
#include <utility>

#include "cpp_timer.h"

namespace stonks::cpp {
TimerBuilder::TimerBuilder(fu2::unique_function<void() const> event)
    : event_{std::move(event)} {}

auto TimerBuilder::Every(absl::Duration event_interval) -> TimerBuilder& {
  Expects(!event_interval_.has_value());
  event_interval_ = event_interval;
  Ensures(event_interval_.has_value());
  return *this;
}

auto TimerBuilder::IfThrowsReattemptEvery(absl::Duration reattempt_interval)
    -> TimerBuilder& {
  Expects(!reattempt_interval_.has_value());
  Expects(event_interval_.has_value());
  reattempt_interval_ = reattempt_interval;
  Ensures(reattempt_interval_.has_value());
  return *this;
}

auto TimerBuilder::IfThrowsReattempt() -> TimerBuilder& {
  return IfThrowsReattemptEvery(absl::ZeroDuration());
}

auto TimerBuilder::Times(int num_reattempts) -> TimerBuilder& {
  Expects(!num_reattempts_.has_value());
  Expects(event_interval_.has_value());
  Expects(reattempt_interval_.has_value());
  num_reattempts_ = num_reattempts;
  Ensures(num_reattempts_.has_value());
  return *this;
}

auto TimerBuilder::Start() -> Timer {
  Expects(event_interval_.has_value());
  return Timer{
      std::move(event_),
      *event_interval_,
      {.reattempt_interval = reattempt_interval_.value_or(absl::ZeroDuration()),
       .num_reattempts = num_reattempts_.value_or(0)}};
}
}  // namespace stonks::cpp