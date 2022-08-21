#include "stonks_order_proxy.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <boost/uuid/detail/uuid_x86.ipp>
#include <chrono>
#include "ccutils_not_null.h"
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/subrange.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>

#include "finance_enums.h"
#include "utils.h"

namespace stonks::finance {
Order OrderProxy::RecordStrategyOrderRequest(
    const StrategyOrderRequest &strategy_order_request) {
  auto new_order = Order{};

  {
    const auto lock = std::lock_guard{orders_mutex_};

    auto &order = orders_.emplace_back();
    order.request_time = utils::GetUnixTime();
    order.uuid = strategy_order_request.order_uuid;
    order.symbol = strategy_order_request.symbol;
    order.buy_or_sell = strategy_order_request.buy_or_sell;
    order.amount = strategy_order_request.amount;
    order.order_type = strategy_order_request.order_type;
    order.strategy_data = strategy_order_request.strategy_data;

    new_order = order;

    orders_cond_var_.notify_all();
  }

  return new_order;
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

    orders_cond_var_.notify_all();
  }

  if (order_update_received_callback_) {
    for (const auto &order_update : order_updates) {
      order_update_received_callback_(order_update);
    }
  }
}

void OrderProxy::SetOrderUpdateReceivedCallback(
    OrderUpdateReceivedCallback order_update_received_callback) {
  order_update_received_callback_ = std::move(order_update_received_callback);
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

std::optional<Order> OrderProxy::FindOrderByUuid(
    boost::uuids::uuid order_uuid) const {
  const auto search_by_uuid = [order_uuid](const Order &order) {
    return order.uuid == order_uuid;
  };

  {
    const auto lock = std::lock_guard{orders_mutex_};

    const auto order = ranges::find_if(orders_, search_by_uuid);

    if (order != orders_.end()) {
      return *order;
    }
  }

  return std::nullopt;
}

std::vector<Order> OrderProxy::GetOpenOrders() const {
  const auto filter_by_status = [](const Order &order) { return true; };

  {
    const auto lock = std::lock_guard{orders_mutex_};
    return orders_ | ranges::views::filter(filter_by_status) |
           ranges::to_vector;
  }
}

namespace {
double CalcBaseBalanceChange(const Order &order) {
  const auto filled_order_info = order.GetLastFilledOrderInfo();

  if (!filled_order_info.has_value()) {
    return 0;
  }

  if (order.buy_or_sell == stonks::finance::BuyOrSell::kBuy) {
    return filled_order_info->executed_amount;
  }

  return -filled_order_info->executed_amount;
}

double CalcQuoteBalanceChange(const Order &order) {
  const auto filled_order_info = order.GetLastFilledOrderInfo();

  if (!filled_order_info.has_value()) {
    return 0;
  }

  const auto value = filled_order_info->executed_quote_amount;

  if (order.buy_or_sell == stonks::finance::BuyOrSell::kSell) {
    return value;
  }

  return -value;
}

double CalcAssetBalanceChange(std::string_view asset, const Order &order) {
  if (asset == order.symbol.base_asset) {
    return CalcBaseBalanceChange(order);
  }

  if (asset == order.symbol.quote_asset) {
    return CalcQuoteBalanceChange(order);
  }

  return 0;
}
}  // namespace

std::vector<TimeDouble> OrderProxy::CalcBalanceHistory(
    std::string_view balance_asset,
    std::optional<std::string_view> second_asset, int drop_first) const {
  const auto filter_orders_by_asset = [balance_asset,
                                       second_asset](const Order &order) {
    if (second_asset.has_value()) {
      return ((order.symbol.base_asset == balance_asset) &&
              (order.symbol.quote_asset == *second_asset)) ||
             ((order.symbol.base_asset == *second_asset) &&
              (order.symbol.quote_asset == balance_asset));
    }

    return (order.symbol.base_asset == balance_asset) ||
           (order.symbol.quote_asset == balance_asset);
  };
  const auto enough_orders = [drop_first, &orders = orders_,
                              &filter_orders_by_asset]() {
    return !(orders | ranges::views::filter(filter_orders_by_asset) |
             ranges::views::drop(drop_first))
                .empty();
  };

  auto balance_history = std::vector<TimeDouble>{};

  const auto calc_balance_changes = [balance_asset,
                                     &balance_history](const Order &order) {
    const auto balance =
        balance_history.empty() ? 0 : balance_history.back().value;
    const auto balance_change = CalcAssetBalanceChange(balance_asset, order);

    balance_history.emplace_back(TimeDouble{.time = order.request_time,
                                            .value = balance + balance_change});
  };

  {
    auto lock = std::unique_lock{orders_mutex_};
    orders_cond_var_.wait(lock, enough_orders);

    ranges::for_each(orders_ | ranges::views::filter(filter_orders_by_asset),
                     calc_balance_changes);
  }

  return balance_history;
}

// TODO
std::vector<StrategyOrderRequest> OrderProxy::FindOrderRequests(
    std::string_view strategy_name, const Symbol &symbol,
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
}  // namespace stonks::finance