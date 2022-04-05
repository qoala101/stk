#include "finance_api.h"

#include <spdlog/spdlog.h>

#include <gsl/gsl>
#include <range/v3/action/push_back.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_api.h"
#include "finance_conversions.h"
#include "finance_enum_conversions.h"

namespace {
int CalculateNumIntervalsBetweenTimes(stonks::finance::Interval interval,
                                      std::chrono::milliseconds start_time,
                                      std::chrono::milliseconds end_time) {
  Expects(interval != stonks::finance::Interval::kInvalid);
  Expects(start_time <= end_time);

  const auto period_ms = (end_time - start_time).count();

  if (period_ms == 0) {
    return 1;
  }

  const auto interval_ms =
      stonks::finance::ConvertIntervalToMilliseconds(interval).count();
  return static_cast<int>(std::ceil(static_cast<double>(period_ms) /
                                    static_cast<double>(interval_ms)));
}
}  // namespace

namespace stonks::finance {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval,
    std::chrono::milliseconds history_start_time,
    std::chrono::milliseconds history_end_time) {
  Expects(interval != Interval::kInvalid);
  Expects(history_start_time <= history_end_time);

  const auto num_candlesticks = CalculateNumIntervalsBetweenTimes(
      interval, history_start_time, history_end_time);

  static const auto kMaxNumCandlesticks = 10000;

  if (num_candlesticks > kMaxNumCandlesticks) {
    spdlog::error("Too many candlesticks requested: {}, maximum {}",
                  num_candlesticks, kMaxNumCandlesticks);
    return std::nullopt;
  }

  auto candlesticks = std::vector<Candlestick>{};
  candlesticks.reserve(num_candlesticks);

  while (candlesticks.size() < num_candlesticks) {
    if (!candlesticks.empty()) {
      history_start_time = candlesticks.back().close_time;
    }

    const auto num_klines_to_request = num_candlesticks - candlesticks.size();
    const auto klines =
        binance::GetKlines(symbol, interval, history_start_time,
                           history_end_time, num_klines_to_request);

    if (!klines.has_value()) {
      spdlog::error("Cannot get candlesticks");
      return std::nullopt;
    }

    if (klines->empty()) {
      spdlog::error("No candlesticks received");
      return std::nullopt;
    }

    candlesticks |= ranges::actions::push_back(
        *klines | ranges::views::transform(ParseCandlestickFromBinanceKline));
  }

  return candlesticks;
}
}  // namespace stonks::finance