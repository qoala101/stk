#include "finance_api.h"

#include <spdlog/spdlog.h>

#include <gsl/gsl>
#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_api.h"
#include "finance_conversions.h"

namespace stonks::finance {
std::optional<std::vector<Candlestick>> GetCandlesticks(
    std::string_view symbol, Interval interval, int64_t history_start_time,
    int64_t history_end_time) {
  Expects(history_start_time <= history_end_time);

  const auto klines = binance::GetKlines(symbol, interval, history_start_time,
                                         history_end_time, 10);

  if (!klines.has_value()) {
    spdlog::error("Cannot get candlesticks");
    return std::nullopt;
  }

  return *klines | ranges::views::transform(ParseCandlestickFromBinanceKline) |
         ranges::to_vector;
}
}  // namespace stonks::finance