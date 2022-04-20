#ifndef STONKS_INCLUDE_BREAKOUT_STRATEGY_H_
#define STONKS_INCLUDE_BREAKOUT_STRATEGY_H_

#include <optional>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class BreakoutStrategy {
 public:
  explicit BreakoutStrategy(double min_volume);

  std::optional<StrategyOrderRequest> ProcessNewCandles(
      const std::vector<Candle> &candles);

 private:
  const double min_volume_{};
  std::optional<Candle> prev_candle_{};
};
}  // namespace stonks::finance

#endif  // STONKS_INCLUDE_BREAKOUT_STRATEGY_H_
