#include "candlesticks_stream.h"

#include <spdlog/spdlog.h>

#include <gsl/assert>
#include <thread>

#include "finance_api.h"
#include "finance_enum_conversions.h"
#include "utils.h"

namespace stonks::finance {
CandlesticksStream::CandlesticksStream(
    Symbol symbol, Interval interval, std::chrono::milliseconds start_time,
    std::optional<std::chrono::milliseconds> end_time)
    : symbol_{std::move(symbol)},
      interval_{interval},
      start_time_{CeilStartTimeToInterval(start_time, interval_)},
      end_time_{end_time} {
  Expects(interval != Interval::kInvalid);
  Expects(!end_time.has_value() || (start_time < end_time));

  if (end_time_.has_value()) {
    end_time_ = FloorEndTimeToInterval(*end_time_, interval_);
  }
}

std::vector<Candlestick> CandlesticksStream::GetNextCandlesticks() {
  if (end_time_.has_value() && (end_time_ < start_time_)) {
    return std::vector<Candlestick>{};
  }

  const auto next_candlestick_end_time =
      start_time_ + ConvertIntervalToMilliseconds(interval_) -
      std::chrono::milliseconds{1};
  std::this_thread::sleep_until(
      std::chrono::system_clock::time_point{next_candlestick_end_time});

  auto candlesticks = std::optional<std::vector<Candlestick>>{};

  while (!candlesticks.has_value()) {
    candlesticks =
        end_time_.has_value() && (*end_time_ <= utils::GetUnixTime())
            ? GetCandlesticks(symbol_, interval_, start_time_, *end_time_)
            : GetCandlesticks(symbol_, interval_, start_time_);
  }

  assert(!candlesticks->empty());

  start_time_ = candlesticks->back().close_time + std::chrono::milliseconds{1};

  return *candlesticks;
}
}  // namespace stonks::finance