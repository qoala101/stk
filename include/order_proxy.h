#ifndef STONKS_ORDER_PROXY_H_
#define STONKS_ORDER_PROXY_H_

#include <string_view>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class OrderProxy {
 public:
  void RecordStrategyOrderRequest(StrategyOrderRequest strategy_order_request);

  std::vector<StrategyOrderRequest> FindRecords(std::string_view strategy_name,
                                                const Symbol &symbol) const;

 private:
  std::vector<StrategyOrderRequest> records_;
};
}  // namespace stonks::finance

#endif  // STONKS_ORDER_PROXY_H_
