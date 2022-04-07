#include "order_proxy.h"

#include <range/v3/to_container.hpp>
#include <range/v3/view/filter.hpp>

namespace stonks::finance {
void OrderProxy::RecordStrategyOrderRequest(
    StrategyOrderRequest strategy_order_request) {
  records_.emplace_back(std::move(strategy_order_request));
}

std::vector<StrategyOrderRequest> OrderProxy::FindRecords(
    std::string_view strategy_name, const Symbol &symbol) const {
  const auto filter_records = [&strategy_name,
                               &symbol](const StrategyOrderRequest &record) {
    return (record.strategy_info.name == strategy_name) &&
           (record.order_request.symbol == symbol);
  };
  return records_ | ranges::views::filter(filter_records) | ranges::to_vector;
}
}  // namespace stonks::finance