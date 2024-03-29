/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "cpp_timer_builder.h"

#include <absl/time/time.h>

#include <gsl/assert>
#include <limits>
#include <utility>

#include "cpp_timer.h"

namespace vh::cpp {
TimerBuilder::TimerBuilder(fu2::unique_function<void()> event)
    : event_{(Expects(!event.empty()), std::move(event))} {
  Ensures(!event_.empty());
}

auto TimerBuilder::Every(absl::Duration event_interval) -> TimerBuilder& {
  Expects(!event_interval_.has_value());
  event_interval_ = event_interval;
  Ensures(event_interval_.has_value());
  return *this;
}

auto TimerBuilder::Once() -> TimerBuilder& {
  return Every(absl::InfiniteDuration());
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
  Expects(!event_.empty());
  Expects(event_interval_.has_value());

  const auto reattempt_interval_set = reattempt_interval_.has_value();
  const auto num_reattempts =
      reattempt_interval_set
          ? num_reattempts_.value_or(std::numeric_limits<int>::max())
          : 0;
  auto timer = Timer{{std::move(event_),
                      *event_interval_,
                      {.reattempt_interval =
                           reattempt_interval_.value_or(absl::ZeroDuration()),
                       .num_reattempts = num_reattempts}}};

  Ensures(event_.empty());
  return timer;
}

auto Execute(fu2::unique_function<void()> event) -> TimerBuilder {
  return TimerBuilder{std::move(event)};
}
}  // namespace vh::cpp