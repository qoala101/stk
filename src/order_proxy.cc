#include "order_proxy.h"

#include <range/v3/to_container.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>

namespace stonks::finance {
void OrderProxy::RecordStrategyOrderRequest(
    StrategyOrderRequest strategy_order_request) {
  {
    const auto lock = std::lock_guard{mutex_};
    records_.emplace_back(std::move(strategy_order_request));
  }

  cond_var_.notify_all();
}

std::vector<StrategyOrderRequest> OrderProxy::FindRecords(
    std::string_view strategy_name, const Symbol &symbol,
    int drop_first) const {
  auto result = std::vector<StrategyOrderRequest>{};

  const auto result_not_empty_condition = [&records = records_, &strategy_name,
                                           &symbol, drop_first, &result]() {
    const auto filter_records = [&strategy_name,
                                 &symbol](const StrategyOrderRequest &record) {
      return (record.strategy_info.name == strategy_name) &&
             (record.order_request.symbol == symbol);
    };

    result = records | ranges::views::filter(filter_records) |
             ranges::views::drop(drop_first) | ranges::to_vector;
    return !result.empty();
  };

  auto lock = std::unique_lock{mutex_};
  cond_var_.wait(lock, result_not_empty_condition);

  return result;
}
}  // namespace stonks::finance