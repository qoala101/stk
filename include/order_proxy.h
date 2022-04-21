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
   * @remark Copy of the order is returned because orders could be updated from
   * the other thread.
   */
  Order RecordStrategyOrderRequest(
      const StrategyOrderRequest &strategy_order_request);

  /**
   * @brief Adds order updates to the corresponding lists of updates.
   */
  void UpdateOrders(const std::vector<OrderMonitorOrderUpdate> &order_updates);

  /**
   * @remark Copies of the orders are returned because orders could be updated
   * from the other thread.
   */
  std::vector<Order> GetAllOrders(int drop_first = 0) const;

  /**
   * @brief Gives list of orders which have not yet received any type of closing
   * update.
   * @remark Copies of the orders are returned because orders could be updated
   * from the other thread.
   */
  std::vector<Order> GetOpenOrders() const;

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
