#ifndef STONKS_ORDER_PROXY_H_
#define STONKS_ORDER_PROXY_H_

#include <boost/uuid/uuid.hpp>
#include <condition_variable>
#include <gsl/pointers>
#include <mutex>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class OrderProxy {
 public:
  /**
   * @brief Adds order request to the list of all orders.
   */
  Order &RecordStrategyOrderRequest(
      const StrategyOrderRequest &strategy_order_request);

  /**
   * @brief Adds order update to the order list of updates.
   */
  void UpdateOrder(const OrderMonitorOrderUpdate &order_update);

  /**
   * @brief Gives list of orders which have not yet received any type of closing
   * update.
   */
  std::vector<gsl::not_null<const Order *>> GetOpenOrders() const;

  // TODO
  std::vector<StrategyOrderRequest> FindOrderRequests(
      std::string_view strategy_name, const Symbol &symbol,
      int drop_first = 0) const;

  // TODO
  std::vector<TimeDouble> CalcBalanceHistory(std::string_view asset,
                                             int drop_first = 0) const;

 private:
  std::vector<Order> orders_{};
  mutable std::mutex orders_mutex_{};
  mutable std::condition_variable orders_cond_var_{};
};
}  // namespace stonks::finance

#endif  // STONKS_ORDER_PROXY_H_
