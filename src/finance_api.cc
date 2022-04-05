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
int64_t CalculateNumIntervalsBetweenTimes(stonks::finance::Interval interval,
                                          int64_t start_time_ms,
                                          int64_t end_time_ms) {
  Expects(interval != stonks::finance::Interval::kInvalid);
  Expects(start_time_ms <= end_time_ms);

  const auto period = end_time_ms - start_time_ms;

  if (period == 0) {
    return 1;
  }

  const auto interval_ms = stonks::finance::ConvertIntervalToMillis(interval);
  return static_cast<int64_t>(
      std::ceil(static_cast<double>(period) / static_cast<double>(interval_ms)));
}
}  // namespace

namespace stonks::finance {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval, int64_t history_start_time_ms,
    int64_t history_end_time_ms) {
  Expects(interval != Interval::kInvalid);
  Expects(history_start_time_ms <= history_end_time_ms);

  const auto num_candlesticks = CalculateNumIntervalsBetweenTimes(
      interval, history_start_time_ms, history_end_time_ms);

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
      history_start_time_ms = candlesticks.back().close_time;
    }

    const auto num_klines_to_request = num_candlesticks - candlesticks.size();
    const auto klines =
        binance::GetKlines(symbol, interval, history_start_time_ms,
                           history_end_time_ms, num_klines_to_request);

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