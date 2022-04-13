#include "candlesticks_stream.h"

#include <spdlog/spdlog.h>

#include <gsl/gsl_assert>
#include <range/v3/to_container.hpp>
#include <range/v3/view/take_while.hpp>
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
      start_time_{start_time},
      end_time_{end_time} {
  Expects(interval_ != Interval::kInvalid);
  Expects(!end_time.has_value() || (start_time < end_time));
}

std::optional<std::vector<Candlestick>>
CandlesticksStream::GetNextCandlesticks() {
  if (const auto stream_finished =
          end_time_.has_value() && (start_time_ > *end_time_)) {
    return std::vector<Candlestick>{};
  }

  const auto time_till_start_time = start_time_ - utils::GetUnixTime();

  if (const auto start_time_not_reached =
          time_till_start_time > std::chrono::milliseconds{0}) {
    std::this_thread::sleep_for(time_till_start_time);
  }

  if (const auto end_time_reached =
          end_time_.has_value() && (*end_time_ <= utils::GetUnixTime())) {
    const auto past_candlesticks =
        GetCandlesticks(symbol_.base_asset + symbol_.quote_asset, interval_,
                        start_time_, end_time_);

    if (!past_candlesticks.has_value()) {
      spdlog::error("Cannot get candlesticks");
      return std::nullopt;
    }

    start_time_ = *end_time_ + std::chrono::milliseconds{1};

    return past_candlesticks;
  }

  const auto future_candlesticks = GetCandlesticks(
      symbol_.base_asset + symbol_.quote_asset, interval_, start_time_);

  if (!future_candlesticks.has_value()) {
    spdlog::error("Cannot get candlesticks");
    return std::nullopt;
  }

  if (const auto need_to_wait_for_next_candlestick =
          future_candlesticks->empty()) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds{ConvertIntervalToMilliseconds(interval_)});
    return GetNextCandlesticks();
  }

  if (!end_time_.has_value()) {
    start_time_ = future_candlesticks->back().close_time;

    return future_candlesticks;
  }

  if (const auto no_past_end_time_candlesticks =
          future_candlesticks->back().open_time <= *end_time_) {
    start_time_ = future_candlesticks->back().close_time;

    return future_candlesticks;
  }

  start_time_ = *end_time_ + std::chrono::milliseconds{1};

  const auto open_time_less_than_end_time =
      [&end_time = end_time_](const Candlestick &candlestick) {
        return candlestick.open_time < *end_time;
      };

  return *future_candlesticks |
         ranges::views::take_while(open_time_less_than_end_time) |
         ranges::to_vector;
}
}  // namespace stonks::finance