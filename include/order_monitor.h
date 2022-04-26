#ifndef STONKS_ORDER_MONITOR_H_
#define STONKS_ORDER_MONITOR_H_

#include <atomic>
#include <condition_variable>
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

  /**
   * @brief Stops monitoring thread.
   */
  ~OrderMonitor();

  /**
   * @brief Starts monitoring thread.
   */
  void Start();

  /**
   * @brief Stops monitoring thread.
   */
  void Stop();

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
  std::mutex monitored_orders_mutex_{};
  std::condition_variable monitored_orders_cond_var_{};

  std::thread monitor_thread_{};
  std::atomic_bool keep_monitoring_{};
};
}  // namespace stonks::finance

#endif  // STONKS_ORDER_MONITOR_H_
