#ifndef STONKS_STONKS_STONKS_ORDER_PROXY_H_
#define STONKS_STONKS_STONKS_ORDER_PROXY_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <string_view>
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

  using OrderUpdateReceivedCallback =
      std::function<void(OrderMonitorOrderUpdate)>;

  /**
   * @brief Set callback to be called when order update is received.
   */
  void SetOrderUpdateReceivedCallback(
      OrderUpdateReceivedCallback order_update_received_callback);

  /**
   * @remark Copies of the orders are returned because orders could be updated
   * from the other thread.
   */
  std::vector<Order> GetAllOrders(int drop_first = 0) const;

  /**
   * @return Nullopt if order doesn't exist.
   */
  std::optional<Order> FindOrderByUuid(boost::uuids::uuid order_uuid) const;

  /**
   * @brief Gives list of orders which have not yet received any type of closing
   * update.
   * @remark Copies of the orders are returned because orders could be updated
   * from the other thread.
   */
  std::vector<Order> GetOpenOrders() const;

  /**
   * @param balance_asset Asset for which balance is calculated.
   * @param second_asset If specified, only operations performed with this asset
   * would count.
   */
  std::vector<TimeDouble> CalcBalanceHistory(
      std::string_view balance_asset,
      std::optional<std::string_view> second_asset = std::nullopt,
      int drop_first = 0) const;

  // TODO
  std::vector<StrategyOrderRequest> FindOrderRequests(
      std::string_view strategy_name, const Symbol &symbol,
      int drop_first = 0) const;

 private:
  std::vector<Order> orders_{};
  mutable std::mutex orders_mutex_{};
  mutable std::condition_variable orders_cond_var_{};

  OrderUpdateReceivedCallback order_update_received_callback_{};
};
}  // namespace stonks::finance

#endif  // STONKS_STONKS_STONKS_ORDER_PROXY_H_
