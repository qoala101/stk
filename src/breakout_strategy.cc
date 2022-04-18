#include "breakout_strategy.h"

#include <gsl/assert>

namespace stonks::finance {
namespace {
std::optional<OrderRequest> ProcessLastCandles(const Candle &prev_candle,
                                               const Candle &new_candle,
                                               double min_volume) {
  Expects(prev_candle.data.has_value());
  Expects(new_candle.data.has_value());
  Expects(min_volume >= 0);

  if (new_candle.data->volume > min_volume) {
    if (new_candle.data->close_price > prev_candle.data->high_price) {
      return OrderRequest{
          .time = new_candle.close_time,
          .buy_or_sell = BuyOrSell::kBuy,
          .quantity = 1,
          .price =
              (new_candle.data->open_price + new_candle.data->close_price) / 2};
    }

    if (new_candle.data->close_price < prev_candle.data->low_price) {
      return OrderRequest{
          .time = new_candle.close_time,
          .buy_or_sell = BuyOrSell::kSell,
          .quantity = 1,
          .price =
              (new_candle.data->open_price + new_candle.data->close_price) / 2};
    }
  }

  return std::nullopt;
}
}  // namespace

BreakoutStrategy::BreakoutStrategy(double min_volume)
    : min_volume_{min_volume} {
  Expects(min_volume >= 0);
}

std::optional<OrderRequest> BreakoutStrategy::ProcessNewCandles(
    const std::vector<Candle> &candles) {
  if (candles.empty()) {
    return std::nullopt;
  }

  if (const auto last_new_has_no_data = !candles.back().data.has_value()) {
    prev_candle_ = std::nullopt;
    return std::nullopt;
  }

  if (candles.size() == 1) {
    if (!prev_candle_.has_value()) {
      prev_candle_ = candles.back();
      return std::nullopt;
    }

    const auto result =
        ProcessLastCandles(*prev_candle_, candles.back(), min_volume_);

    prev_candle_ = candles.back();
    return result;
  }

  const auto &new_prev = *std::prev(candles.end(), 2);
  prev_candle_ = candles.back();

  if (const auto new_prev_has_no_data = !new_prev.data.has_value()) {
    return std::nullopt;
  }

  return ProcessLastCandles(new_prev, *prev_candle_, min_volume_);
}
}  // namespace stonks::finance