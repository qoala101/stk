#include "order_monitor.h"

#include <future>
#include <thread>

namespace stonks::finance {
OrderMonitor::OrderMonitor(OrdersUpdatedCallback orders_updated_callback)
    : orders_updated_callback_{std::move(orders_updated_callback)} {}

void OrderMonitor::MonitorOrders(
    const std::vector<OrderProxyMonitorRequest> &monitor_requests) {}

void OrderMonitor::ProcessOrderRequest(
    const OrderProxyOrderRequest &order_request) {}
}  // namespace stonks::finance