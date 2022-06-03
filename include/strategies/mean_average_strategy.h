#ifndef STONKS_STRATEGIES_MEAN_AVERAGE_STRATEGY_H_
#define STONKS_STRATEGIES_MEAN_AVERAGE_STRATEGY_H_

#include <boost/uuid/uuid.hpp>
#include <mutex>
#include <optional>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class MeanAverageStrategy {
 public:
  explicit MeanAverageStrategy(double base_precision, double price_precision,
                               double comission, double profit);

  std::optional<StrategyOrderRequest> ProcessNewPrices(
      const std::vector<TimeDouble> &prices);

  std::optional<StrategyOrderRequest> ProcessOrderUpdate(
      const OrderProxyToStrategyOrderUpdate &order_update);

  std::vector<TimeDouble> GetAllPrices() const;

 private:
  std::vector<TimeDouble> prices_{};
  mutable std::mutex prices_mutex_{};

  const double base_precision_{};
  const double price_precision_{};
  const double comission_{};
  const double profit_{};

  boost::uuids::uuid buy_order_uuid_{};
  boost::uuids::uuid sell_order_uuid_{};
  mutable std::mutex order_uuids_mutex_{};

  double sell_order_price_{};
  double sell_order_amount_{};
};
}  // namespace stonks::finance

#endif  // STONKS_STRATEGIES_MEAN_AVERAGE_STRATEGY_H_
