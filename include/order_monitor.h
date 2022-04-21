#ifndef STONKS_ORDER_MONITOR_H_
#define STONKS_ORDER_MONITOR_H_

#include <atomic>
#include <functional>
#include <shared_mutex>
#include <thread>
#include <vector>

#include "finance_types.h"

namespace stonks::finance {
class OrderMonitor {
 public:
  using OrdersUpdatedCallback =
      std::function<void(const std::vector<OrderMonitorOrderUpdate> &)>;

  /**
   * @param orders_updated_callback Function to be called when monitor observes
   * order update.
   */
  explicit OrderMonitor(OrdersUpdatedCallback orders_updated_callback);

  ~OrderMonitor();

  /**
   * @brief Adds requested orders to monitoring.
   */
  void MonitorOrders(
      const std::vector<OrderProxyMonitorRequest> &monitor_requests);

  /**
   * @brief Places the order and adds it to monitoring.
   */
  void ProcessOrderRequest(const OrderProxyOrderRequest &order_request);

 private:
  OrdersUpdatedCallback orders_updated_callback_{};
  std::vector<OrderMonitorOrderState> monitored_orders_{};
  std::shared_mutex monitored_orders_mutex_{};
  std::atomic_bool keep_monitoring_{};
  std::thread monitor_thread_{};
};
}  // namespace stonks::finance

#endif  // STONKS_ORDER_MONITOR_H_
