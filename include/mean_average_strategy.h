#ifndef STONKS_MEAN_AVERAGE_STRATEGY_H_
#define STONKS_MEAN_AVERAGE_STRATEGY_H_

#include <mutex>
#include <optional>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class MeanAverageStrategy {
 public:
  std::optional<StrategyOrderRequest> ProcessNewPrices(
      const std::vector<TimeDouble> &prices);

  std::vector<TimeDouble> GetAllPrices() const;

 private:
  std::vector<TimeDouble> prices_{};
  mutable std::mutex prices_mutex_{};
};
}  // namespace stonks::finance

#endif  // STONKS_MEAN_AVERAGE_STRATEGY_H_
