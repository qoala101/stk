#include "breakout_strategy.h"

#include <gsl/assert>

namespace stonks::finance {
namespace {
std::optional<OrderRequest> ProcessLastCandlesticks(
    const Candlestick &prev_candlestick, const Candlestick &new_candlestick,
    double min_volume) {
  Expects(prev_candlestick.data.has_value());
  Expects(new_candlestick.data.has_value());
  Expects(min_volume >= 0);

  if (new_candlestick.data->volume > min_volume) {
    if (new_candlestick.data->close_price > prev_candlestick.data->high_price) {
      return OrderRequest{.time = new_candlestick.close_time,
                          .buy_or_sell = BuyOrSell::kBuy,
                          .quantity = 1,
                          .price = (new_candlestick.data->open_price +
                                    new_candlestick.data->close_price) /
                                   2};
    }

    if (new_candlestick.data->close_price < prev_candlestick.data->low_price) {
      return OrderRequest{.time = new_candlestick.close_time,
                          .buy_or_sell = BuyOrSell::kSell,
                          .quantity = 1,
                          .price = (new_candlestick.data->open_price +
                                    new_candlestick.data->close_price) /
                                   2};
    }
  }

  return std::nullopt;
}
}  // namespace

BreakoutStrategy::BreakoutStrategy(double min_volume)
    : min_volume_{min_volume} {
  Expects(min_volume >= 0);
}

std::optional<OrderRequest> BreakoutStrategy::ProcessNewCandlesticks(
    const std::vector<Candlestick> &candlesticks) {
  if (candlesticks.empty()) {
    return std::nullopt;
  }

  if (const auto last_new_has_no_data = !candlesticks.back().data.has_value()) {
    prev_candlestick_ = std::nullopt;
    return std::nullopt;
  }

  if (candlesticks.size() == 1) {
    if (!prev_candlestick_.has_value()) {
      prev_candlestick_ = candlesticks.back();
      return std::nullopt;
    }

    const auto result = ProcessLastCandlesticks(
        *prev_candlestick_, candlesticks.back(), min_volume_);

    prev_candlestick_ = candlesticks.back();
    return result;
  }

  const auto &new_prev = *std::prev(candlesticks.end(), 2);
  prev_candlestick_ = candlesticks.back();

  if (const auto new_prev_has_no_data = !new_prev.data.has_value()) {
    return std::nullopt;
  }

  return ProcessLastCandlesticks(new_prev, *prev_candlestick_, min_volume_);
}
}  // namespace stonks::finance