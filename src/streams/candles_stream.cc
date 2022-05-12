#include "candles_stream.h"

#include <spdlog/spdlog.h>

#include <gsl/assert>
#include <thread>

#include "finance_api.h"
#include "finance_enum_conversions.h"
#include "utils.h"

namespace stonks::finance {
CandlesStream::CandlesStream(Symbol symbol, Interval interval,
                             std::chrono::milliseconds start_time,
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

std::vector<Candle> CandlesStream::GetNextCandles() {
  if (end_time_.has_value() && (end_time_ < start_time_)) {
    return std::vector<Candle>{};
  }

  const auto next_candle_end_time = start_time_ +
                                    ConvertIntervalToMilliseconds(interval_) -
                                    std::chrono::milliseconds{1};
  std::this_thread::sleep_until(
      std::chrono::system_clock::time_point{next_candle_end_time});

  auto candles = std::optional<std::vector<Candle>>{};

  while (!candles.has_value()) {
    candles = end_time_.has_value() && (*end_time_ <= utils::GetUnixTime())
                  ? GetCandles(symbol_, interval_, start_time_, *end_time_)
                  : GetCandles(symbol_, interval_, start_time_);
  }

  assert(!candles->empty());

  start_time_ = candles->back().close_time + std::chrono::milliseconds{1};

  return *candles;
}
}  // namespace stonks::finance