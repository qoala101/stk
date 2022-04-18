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

  std::vector<StrategyOrderRequest> FindOrderRequests(
      std::string_view strategy_name, const Symbol &symbol,
      int drop_first = 0) const;

  std::vector<TimeDouble> CalcBalanceHistory(std::string_view asset,
                                             int drop_first = 0) const;

 private:
  std::vector<StrategyOrderRequest> order_requests_{};
  mutable std::condition_variable order_requests_cond_var_{};
  mutable std::mutex order_requests_mutex_{};

  std::vector<std::string> order_responses_{}; // TODO
};
}  // namespace stonks::finance

#endif  // STONKS_ORDER_PROXY_H_
