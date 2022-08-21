#include "stonks_order_monitor.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <gsl/pointers>
#include <gsl/util>
#include <optional>
#include <thread>
#include <utility>

#include "ccutils_not_null.h"
#include "finance_api.h"
#include "utils.h"

namespace stonks::finance {
OrderMonitor::OrderMonitor(OrdersUpdatedCallback orders_updated_callback)
    : orders_updated_callback_{std::move(orders_updated_callback)},
      keep_monitoring_{true} {}

OrderMonitor::~OrderMonitor() {
  Stop();
  monitor_thread_.join();
}

void OrderMonitor::Start() {
  if (const auto already_started = monitor_thread_.joinable()) {
    return;
  }

  monitor_thread_ =
      std::thread{[&keep_monitoring = keep_monitoring_,
                   &monitored_orders = monitored_orders_,
                   &monitored_orders_mutex = monitored_orders_mutex_,
                   &monitored_orders_cond_var = monitored_orders_cond_var_,
                   &orders_updated_callback = orders_updated_callback_]() {
        while (keep_monitoring.load(std::memory_order_relaxed)) {
          const auto has_orders_to_monitor = [&monitored_orders]() {
            return !monitored_orders.empty();
          };
          auto order_updates = std::vector<OrderMonitorOrderUpdate>{};

          {
            auto lock = std::unique_lock{monitored_orders_mutex};
            monitored_orders_cond_var.wait(lock, has_orders_to_monitor);

            auto stop_monitoring_orders =
                std::vector<ccutils::Nn<const OrderMonitorOrderState *>>{};

            for (auto &monitored_order : monitored_orders) {
              const auto sleep_finally = gsl::finally([]() {
                std::this_thread::sleep_for(std::chrono::seconds{1});
              });

              const auto order_info = GetOrderInfo(monitored_order.symbol,
                                                   monitored_order.order_uuid);

              if (!order_info.has_value()) {
                spdlog::error(
                    "Cannot get order info {}",
                    utils::ConvertUuidToString(monitored_order.order_uuid));
                continue;
              }

              const auto first_update =
                  !monitored_order.last_order_update.has_value();
              const auto order_status_changed =
                  first_update ||
                  (order_info->order_status !=
                   monitored_order.last_order_update->GetOrderStatus());

              if (order_status_changed) {
                auto &order_update = order_updates.emplace_back();
                order_update.order_uuid = monitored_order.order_uuid;
                order_update.order_update.order_update = *order_info;

                if (IsOrderStatusFinal(order_info->order_status)) {
                  stop_monitoring_orders.emplace_back(&monitored_order);
                  continue;
                }
              }

              monitored_order.last_order_update =
                  OrderUpdate{.order_update = *order_info};
            }

            const auto check_if_order_marked_to_be_unmonitored =
                [&stop_monitoring_orders](
                    const OrderMonitorOrderState &monitored_order) {
                  const auto compare_pointers =
                      [&monitored_order](
                          const ccutils::Nn<const OrderMonitorOrderState *>
                              unmonitored_order) {
                        return unmonitored_order == &monitored_order;
                      };

                  const auto marked_to_be_unmonitored =
                      std::find_if(stop_monitoring_orders.begin(),
                                   stop_monitoring_orders.end(),
                                   compare_pointers) !=
                      stop_monitoring_orders.end();
                  return marked_to_be_unmonitored;
                };

            monitored_orders.erase(
                std::remove_if(monitored_orders.begin(), monitored_orders.end(),
                               check_if_order_marked_to_be_unmonitored),
                monitored_orders.end());

            if (!order_updates.empty()) {
              orders_updated_callback(order_updates);
            }
          }
        }
      }};
}

void OrderMonitor::Stop() {
  keep_monitoring_.store(false, std::memory_order::relaxed);
}

void OrderMonitor::MonitorOrders(
    const std::vector<OrderProxyMonitorRequest> &monitor_requests) {
  // TODO
}

void OrderMonitor::ProcessOrderRequest(
    const OrderProxyOrderRequest &order_request) {
  const auto order_error = PlaceOrder(order_request);

  if (order_error.has_value()) {
    auto order_updates = std::vector<OrderMonitorOrderUpdate>{};

    auto &order_update = order_updates.emplace_back();
    order_update.order_uuid = order_request.order_uuid;
    order_update.order_update.order_update = *order_error;

    orders_updated_callback_(order_updates);
    return;
  }

  {
    const auto lock = std::unique_lock{monitored_orders_mutex_};

    auto &monitored_order = monitored_orders_.emplace_back();
    monitored_order.order_uuid = order_request.order_uuid;
    monitored_order.symbol = order_request.symbol;

    monitored_orders_cond_var_.notify_all();
  }
}
}  // namespace stonks::finance