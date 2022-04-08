#ifndef STONKS_ORDER_PROXY_H_
#define STONKS_ORDER_PROXY_H_

#include <condition_variable>
#include <mutex>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class OrderProxy {
 public:
  void RecordStrategyOrderRequest(StrategyOrderRequest strategy_order_request);

  std::vector<StrategyOrderRequest> FindRecords(std::string_view strategy_name,
                                                const Symbol &symbol,
                                                int drop_first = 0) const;

 private:
  std::vector<StrategyOrderRequest> records_{};
  mutable std::condition_variable cond_var_{};
  mutable std::mutex mutex_{};
};
}  // namespace stonks::finance

#endif  // STONKS_ORDER_PROXY_H_
