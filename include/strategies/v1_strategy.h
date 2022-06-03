#ifndef STONKS_STRATEGIES_V1_STRATEGY_H_
#define STONKS_STRATEGIES_V1_STRATEGY_H_

#include <boost/uuid/uuid.hpp>
#include <mutex>
#include <vector>

#include "finance_types.h"
#include "strategy.h"

namespace stonks {
class V1Strategy : public Strategy {
 public:
  explicit V1Strategy();

 private:
  void ProcessEvent(const Event &event) override;

  std::vector<finance::SymbolPriceTick> prices_{};
  mutable std::mutex prices_mutex_{};

  // const double base_precision_{};
  // const double price_precision_{};
  const double comission_{0.00};
  const double profit_{0.000};

  boost::uuids::uuid buy_order_uuid_{};
  boost::uuids::uuid sell_order_uuid_{};
  mutable std::mutex order_uuids_mutex_{};

  double sell_order_price_{};
  double sell_order_amount_{};
};
}  // namespace stonks

#endif  // STONKS_STRATEGIES_V1_STRATEGY_H_
