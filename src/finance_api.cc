#include "finance_api.h"

#include <spdlog/spdlog.h>

#include <gsl/gsl>
#include <thread>

#include "binance_api.h"
#include "finance_conversions.h"
#include "finance_enum_conversions.h"
#include "utils.h"

namespace stonks::finance {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    const Symbol &symbol, Interval interval,
    std::chrono::milliseconds start_time, std::chrono::milliseconds end_time) {
  Expects(interval != Interval::kInvalid);
  Expects(start_time < end_time);

  const auto first_candlestick_open_time =
      CeilStartTimeToInterval(start_time, interval);
  const auto last_candlestick_close_time =
      FloorEndTimeToInterval(end_time, interval);

  if (first_candlestick_open_time > last_candlestick_close_time) {
    return std::vector<Candlestick>{};
  }

  const auto num_candlesticks = CanculateNumIntervalsInPeriod(
      first_candlestick_open_time, last_candlestick_close_time, interval);

  std::this_thread::sleep_until(
      std::chrono::system_clock::time_point{last_candlestick_close_time});

  const auto interval_time = ConvertIntervalToMilliseconds(interval);
  start_time = first_candlestick_open_time;
  end_time = last_candlestick_close_time;
  auto candlesticks = std::vector<Candlestick>{};

  while (candlesticks.size() < num_candlesticks) {
    if (!candlesticks.empty()) {
      start_time =
          candlesticks.back().close_time + std::chrono::milliseconds{1};
    }

    const auto klines = binance::GetKlines(
        symbol.base_asset + symbol.quote_asset, interval, start_time,
        last_candlestick_close_time, std::numeric_limits<int>::max());

    if (!klines.has_value()) {
      spdlog::error("Cannot get candlesticks");
      return std::nullopt;
    }

    const auto last_kline_close_time = klines->empty()
                                           ? last_candlestick_close_time
                                           : klines->back().close_time;
    auto kline = klines->begin();

    for (auto kline_open_time = start_time;
         kline_open_time < last_kline_close_time;
         kline_open_time += interval_time) {
      if (const auto kline_is_valid = (kline != klines->end()) &&
                                      (kline->open_time == kline_open_time)) {
        candlesticks.emplace_back(ParseCandlestickFromBinanceKline(*kline));
        kline++;
        continue;
      }

      auto &null_kline = candlesticks.emplace_back();
      null_kline.open_time = kline_open_time;
      null_kline.close_time =
          kline_open_time + interval_time - std::chrono::milliseconds{1};
    }
  }

  return candlesticks;
}

std::chrono::milliseconds CeilStartTimeToInterval(
    std::chrono::milliseconds time, Interval interval) {
  Expects(interval != Interval::kInvalid);

  const auto interval_ms =
      stonks::finance::ConvertIntervalToMilliseconds(interval).count();
  return std::chrono::milliseconds{
      ((time.count() + interval_ms - 1) / interval_ms) * interval_ms};
}

std::chrono::milliseconds FloorEndTimeToInterval(std::chrono::milliseconds time,
                                                 Interval interval) {
  Expects(interval != Interval::kInvalid);

  const auto interval_ms =
      stonks::finance::ConvertIntervalToMilliseconds(interval).count();
  return std::chrono::milliseconds{
      (((time.count() + 1) / interval_ms) * interval_ms) - 1};
}

int CanculateNumIntervalsInPeriod(std::chrono::milliseconds start_time,
                                  std::chrono::milliseconds end_time,
                                  Interval interval) {
  Expects(interval != Interval::kInvalid);
  Expects(start_time < end_time);

  return (FloorEndTimeToInterval(end_time, interval).count() + 1 -
          CeilStartTimeToInterval(start_time, interval).count()) /
         stonks::finance::ConvertIntervalToMilliseconds(interval).count();
}
}  // namespace stonks::finance