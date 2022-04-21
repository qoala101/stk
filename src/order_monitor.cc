#include "order_monitor.h"

#include <spdlog/spdlog.h>

#include <future>
#include <thread>

#include "finance_api.h"

namespace stonks::finance {
OrderMonitor::OrderMonitor(OrdersUpdatedCallback orders_updated_callback)
    : orders_updated_callback_{std::move(orders_updated_callback)},
      keep_monitoring_{true},
      monitor_thread_{[&keep_monitoring = keep_monitoring_,
                       &monitored_orders = monitored_orders_,
                       &monitored_orders_mutex = monitored_orders_mutex_]() {
        while (keep_monitoring.load(std::memory_order_relaxed)) {
          {
            const auto lock = std::shared_lock{monitored_orders_mutex};

            // for (const auto &monitored_order : monitored_orders) {
            //   const auto order_info = finance::GetOrderInfo(
            //       monitored_order.symbol, monitored_order.order_uuid);

            //   if (!order_info.has_value()) {
            //     spdlog::error(
            //         "Cannot get order info {}",
            //         utils::ConvertUuidToString(monitored_order.order_uuid));
            //     continue;
            //   }
            // }
          }

          std::this_thread::sleep_for(std::chrono::seconds{10});
        }
      }} {}

OrderMonitor::~OrderMonitor() {
  keep_monitoring_.store(false, std::memory_order::relaxed);
  monitor_thread_.join();
}

void OrderMonitor::MonitorOrders(
    const std::vector<OrderProxyMonitorRequest> &monitor_requests) {}

void OrderMonitor::ProcessOrderRequest(
    const OrderProxyOrderRequest &order_request) {
  // const auto order_error = finance::PlaceOrder(order_request);

  // {
  //   const auto lock = std::unique_lock{monitored_orders_mutex_};

  //   auto &monitored_order = monitored_orders_.emplace_back();
  //   monitored_order.order_uuid = order_request.order_uuid;
  //   monitored_order.symbol = order_request.symbol;

  //   if (order_error.has_value()) {
  //     monitored_order.last_order_update =
  //         OrderUpdate{.order_update = *order_error};
  //   }
  // }

  auto updated_orders = std::vector<OrderMonitorOrderUpdate>{};
  auto &order_update = updated_orders.emplace_back();
  order_update.order_uuid = order_request.order_uuid;

  const auto price =
      finance::GetCurrentAverageSymbolPrice(order_request.symbol);

  if (price.has_value()) {
    order_update.order_update.order_update =
        OrderInfo{.order_status = OrderStatus::kFilled,
                  .requested_amount = (float{1} / *price) * float{10},
                  .executed_amount = (float{1} / *price) * float{10},
                  .price = *price};
  } else {
    order_update.order_update.order_update =
        OrderError{.message = "Cannot get average order price."};
  }

  orders_updated_callback_(updated_orders);
}
}  // namespace stonks::finance