#include "finance_api.h"

#include <spdlog/spdlog.h>

#include <gsl/gsl>
#include <range/v3/action/push_back.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>
#include <thread>

#include "binance_api.h"
#include "finance_conversions.h"
#include "finance_enum_conversions.h"
#include "utils.h"

namespace {
int CalculateNumIntervalsBetweenTimes(stonks::finance::Interval interval,
                                      std::chrono::milliseconds start_time,
                                      std::chrono::milliseconds end_time) {
  Expects(interval != stonks::finance::Interval::kInvalid);
  Expects(start_time <= end_time);

  const auto period_ms = (end_time - start_time).count();
  const auto interval_ms =
      stonks::finance::ConvertIntervalToMilliseconds(interval).count();
  return static_cast<int>(std::ceil(static_cast<double>(period_ms) /
                                    static_cast<double>(interval_ms)));
}
}  // namespace

namespace stonks::finance {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval,
    std::chrono::milliseconds start_time,
    std::optional<std::chrono::milliseconds> end_time) {
  Expects(interval != Interval::kInvalid);
  Expects(!end_time.has_value() || (start_time <= *end_time));

  auto candlesticks = std::vector<Candlestick>{};

  while (true) {
    if (!candlesticks.empty()) {
      start_time = candlesticks.back().close_time;
    }

    if (end_time.has_value() && (start_time > *end_time)) {
      break;
    }

    const auto klines =
        binance::GetKlines(symbol, interval, start_time, end_time,
                           std::numeric_limits<int>::max());

    if (!klines.has_value()) {
      spdlog::error("Cannot get candlesticks");
      return std::nullopt;
    }

    if (!klines->empty()) {
      candlesticks |= ranges::actions::push_back(
          *klines | ranges::views::transform(ParseCandlestickFromBinanceKline));
      continue;
    }

    if (!end_time.has_value()) {
      return candlesticks;
    }

    const auto time_till_end_time = *end_time - utils::GetUnixTime();

    if (const auto end_time_in_past =
            time_till_end_time < std::chrono::milliseconds{0}) {
      return candlesticks;
    }

    std::this_thread::sleep_for(time_till_end_time);
  }

  return candlesticks;
}
}  // namespace stonks::finance