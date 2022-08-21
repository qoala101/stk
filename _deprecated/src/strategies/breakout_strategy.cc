#include "breakout_strategy.h"

#include <gsl/assert>
#include <iterator>
#include "ccutils_not_null.h"

#include "finance_enums.h"
#include "utils.h"

namespace stonks::finance {
namespace {
std::optional<StrategyOrderRequest> ProcessLastCandles(
    const Candle &prev_candle, const Candle &new_candle) {
  Expects(prev_candle.data.has_value());
  Expects(new_candle.data.has_value());

  if (new_candle.data->close_price > prev_candle.data->high_price) {
    return StrategyOrderRequest{
        .order_uuid = utils::GenerateUuid(),
        .buy_or_sell = BuyOrSell::kBuy,
        .amount = UnspecifiedAmount{},
        .order_type = MarketOrderType{},
        .strategy_data = BreakoutStrategyData{
            .last_candle_close_time = new_candle.close_time,
            .expected_price =
                (new_candle.data->open_price + new_candle.data->close_price) /
                2}};
  }

  if (new_candle.data->close_price < prev_candle.data->low_price) {
    return StrategyOrderRequest{
        .order_uuid = utils::GenerateUuid(),
        .buy_or_sell = BuyOrSell::kSell,
        .amount = UnspecifiedAmount{},
        .order_type = MarketOrderType{},
        .strategy_data = BreakoutStrategyData{
            .last_candle_close_time = new_candle.close_time,
            .expected_price =
                (new_candle.data->open_price + new_candle.data->close_price) /
                2}};
  }

  return std::nullopt;
}
}  // namespace

std::optional<StrategyOrderRequest> BreakoutStrategy::ProcessNewCandles(
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

    const auto result = ProcessLastCandles(*prev_candle_, candles.back());

    prev_candle_ = candles.back();
    return result;
  }

  const auto &new_prev = *std::prev(candles.end(), 2);
  prev_candle_ = candles.back();

  if (const auto new_prev_has_no_data = !new_prev.data.has_value()) {
    return std::nullopt;
  }

  return ProcessLastCandles(new_prev, *prev_candle_);
}
}  // namespace stonks::finance