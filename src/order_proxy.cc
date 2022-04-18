#include "order_proxy.h"

#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>

namespace stonks::finance {
void OrderProxy::RecordStrategyOrderRequest(
    StrategyOrderRequest strategy_order_request) {
  {
    const auto lock = std::lock_guard{order_requests_mutex_};
    order_requests_.emplace_back(std::move(strategy_order_request));
  }

  order_requests_cond_var_.notify_all();
}

std::vector<StrategyOrderRequest> OrderProxy::FindOrderRequests(
    std::string_view strategy_name, const Symbol &symbol,
    int drop_first) const {
  auto found_requests = std::vector<StrategyOrderRequest>{};

  const auto find_requests_and_check_if_empty =
      [&strategy_name, &symbol, drop_first, &order_requests = order_requests_,
       &found_requests]() {
        const auto filter_requests =
            [&strategy_name, &symbol](const StrategyOrderRequest &record) {
              return (record.strategy_info.name == strategy_name) &&
                     (record.order_request.symbol == symbol);
            };

        found_requests = order_requests |
                         ranges::views::filter(filter_requests) |
                         ranges::views::drop(drop_first) | ranges::to_vector;
        return !found_requests.empty();
      };

  {
    auto lock = std::unique_lock{order_requests_mutex_};
    order_requests_cond_var_.wait(lock, find_requests_and_check_if_empty);
  }

  return found_requests;
}

namespace {
double CalcBaseBalanceChange(const OrderRequest &order_request) {
  if (order_request.buy_or_sell == stonks::finance::BuyOrSell::kBuy) {
    return order_request.quantity;
  }

  return -order_request.quantity;
}

double CalcQuoteBalanceChange(const OrderRequest &order_request) {
  const auto value = order_request.quantity * order_request.price;

  if (order_request.buy_or_sell == stonks::finance::BuyOrSell::kSell) {
    return value;
  }

  return -value;
}

double CalcAssetBalanceChange(std::string_view asset,
                              const OrderRequest &order_request) {
  if (asset == order_request.symbol.base_asset) {
    return CalcBaseBalanceChange(order_request);
  }

  if (asset == order_request.symbol.quote_asset) {
    return CalcQuoteBalanceChange(order_request);
  }

  return 0;
}
}  // namespace

std::vector<TimeDouble> OrderProxy::CalcBalanceHistory(std::string_view asset,
                                                       int drop_first) const {
  const auto filter_records_by_asset =
      [asset](const StrategyOrderRequest &request) {
        return (request.order_request.symbol.base_asset == asset) ||
               (request.order_request.symbol.quote_asset == asset);
      };
  const auto enough_records = [drop_first, &order_requests = order_requests_,
                               &filter_records_by_asset]() {
    return !(order_requests | ranges::views::filter(filter_records_by_asset) |
             ranges::views::drop(drop_first))
                .empty();
  };

  auto balance_history = std::vector<TimeDouble>{};

  const auto calc_balance_changes = [asset, &balance_history](
                                        const StrategyOrderRequest &request) {
    const auto balance =
        balance_history.empty() ? 0 : balance_history.back().value;
    const auto balance_change =
        CalcAssetBalanceChange(asset, request.order_request);

    balance_history.emplace_back(TimeDouble{.time = request.order_request.time,
                                            .value = balance + balance_change});
  };

  {
    auto lock = std::unique_lock{order_requests_mutex_};
    order_requests_cond_var_.wait(lock, enough_records);

    ranges::for_each(
        order_requests_ | ranges::views::filter(filter_records_by_asset),
        calc_balance_changes);
  }

  return balance_history;
}
}  // namespace stonks::finance