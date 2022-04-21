#include "order_proxy.h"

#include <spdlog/spdlog.h>

#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "utils.h"

namespace stonks::finance {
Order OrderProxy::RecordStrategyOrderRequest(
    const StrategyOrderRequest &strategy_order_request) {
  auto order = (Order *){nullptr};

  {
    const auto lock = std::lock_guard{orders_mutex_};

    order = &orders_.emplace_back();
    order->request_time = utils::GetUnixTime();
    order->uuid = utils::GenerateUuid();
    order->symbol = strategy_order_request.symbol;
    order->buy_or_sell = strategy_order_request.buy_or_sell;
    order->amount = strategy_order_request.amount;
    order->order_type = strategy_order_request.order_type;
    order->strategy_data = strategy_order_request.strategy_data;
  }

  orders_cond_var_.notify_all();
  return *order;
}

void OrderProxy::UpdateOrders(
    const std::vector<OrderMonitorOrderUpdate> &order_updates) {
  {
    const auto lock = std::lock_guard{orders_mutex_};

    for (const auto &order_update : order_updates) {
      const auto check_uuid = [&order_update](const Order &order) {
        return order.uuid == order_update.order_uuid;
      };

      auto order = std::find_if(orders_.begin(), orders_.end(), check_uuid);

      if (order == orders_.end()) {
        spdlog::error("Cannot update order {} because it's missing.",
                      utils::ConvertUuidToString(order_update.order_uuid));
        continue;
      }

      auto &new_order_update = order->order_updates.emplace_back();
      new_order_update.received_time = utils::GetUnixTime();
      new_order_update.order_update = order_update.order_update;
    }
  }

  orders_cond_var_.notify_all();
}

std::vector<Order> OrderProxy::GetAllOrders(int drop_first) const {
  const auto check_if_enough_orders = [drop_first, &orders = orders_]() {
    return orders.size() > drop_first;
  };

  {
    auto lock = std::unique_lock{orders_mutex_};
    orders_cond_var_.wait(lock, check_if_enough_orders);

    return orders_ | ranges::views::drop(drop_first) | ranges::to_vector;
  }
}

std::vector<Order> OrderProxy::GetOpenOrders() const {
  const auto filter_by_status = [](const Order &order) { return true; };

  {
    const auto lock = std::lock_guard{orders_mutex_};
    return orders_ | ranges::views::filter(filter_by_status) |
           ranges::to_vector;
  }
}

// TODO
std::vector<StrategyOrderRequest> OrderProxy::FindOrderRequests(
    std::string_view strategy_name, const Symbol &symbol,
    int drop_first) const {
  return {};
}

// TODO
std::vector<TimeDouble> OrderProxy::CalcBalanceHistory(std::string_view asset,
                                                       int drop_first) const {
  return {};
}

// std::vector<StrategyOrderRequest> OrderProxy::FindOrderRequests(
//     std::string_view strategy_name, const Symbol &symbol,
//     int drop_first) const {
//   auto found_requests = std::vector<StrategyOrderRequest>{};

//   const auto find_requests_and_check_if_empty = [&strategy_name, &symbol,
//                                                  drop_first,
//                                                  &order_states =
//                                                  order_states_,
//                                                  &found_requests]() {
//     const auto filter_requests = [&strategy_name,
//                                   &symbol](const StrategyOrderRequest
//                                   &record) {
//       return (record.strategy_info.name == strategy_name) &&
//              (record.order_request.symbol == symbol);
//     };

//     found_requests = order_states | ranges::views::filter(filter_requests) |
//                      ranges::views::drop(drop_first) | ranges::to_vector;
//     return !found_requests.empty();
//   };

//   {
//     auto lock = std::unique_lock{order_states_mutex_};
//     order_states_cond_var_.wait(lock, find_requests_and_check_if_empty);
//   }

//   return found_requests;
// }

// namespace {
// double CalcBaseBalanceChange(const OrderRequest &order_request) {
//   if (order_request.buy_or_sell == stonks::finance::BuyOrSell::kBuy) {
//     return order_request.quantity;
//   }

//   return -order_request.quantity;
// }

// double CalcQuoteBalanceChange(const OrderRequest &order_request) {
//   const auto value = order_request.quantity * order_request.price;

//   if (order_request.buy_or_sell == stonks::finance::BuyOrSell::kSell) {
//     return value;
//   }

//   return -value;
// }

// double CalcAssetBalanceChange(std::string_view asset,
//                               const OrderRequest &order_request) {
//   if (asset == order_request.symbol.base_asset) {
//     return CalcBaseBalanceChange(order_request);
//   }

//   if (asset == order_request.symbol.quote_asset) {
//     return CalcQuoteBalanceChange(order_request);
//   }

//   return 0;
// }
// }  // namespace

// std::vector<TimeDouble> OrderProxy::CalcBalanceHistory(std::string_view
// asset,
//                                                        int drop_first) const
//                                                        {
//   const auto filter_records_by_asset =
//       [asset](const StrategyOrderRequest &request) {
//         return (request.order_request.symbol.base_asset == asset) ||
//                (request.order_request.symbol.quote_asset == asset);
//       };
//   const auto enough_records = [drop_first, &order_states = order_states_,
//                                &filter_records_by_asset]() {
//     return !(order_states | ranges::views::filter(filter_records_by_asset) |
//              ranges::views::drop(drop_first))
//                 .empty();
//   };

//   auto balance_history = std::vector<TimeDouble>{};

//   const auto calc_balance_changes = [asset, &balance_history](
//                                         const StrategyOrderRequest &request)
//                                         {
//     const auto balance =
//         balance_history.empty() ? 0 : balance_history.back().value;
//     const auto balance_change =
//         CalcAssetBalanceChange(asset, request.order_request);

//     balance_history.emplace_back(TimeDouble{.time =
//     request.order_request.time,
//                                             .value = balance +
//                                             balance_change});
//   };

//   {
//     auto lock = std::unique_lock{order_states_mutex_};
//     order_states_cond_var_.wait(lock, enough_records);

//     ranges::for_each(
//         order_states_ | ranges::views::filter(filter_records_by_asset),
//         calc_balance_changes);
//   }

//   return balance_history;
// }
}  // namespace stonks::finance