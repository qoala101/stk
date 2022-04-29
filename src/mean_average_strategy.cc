#include "mean_average_strategy.h"

#include <range/v3/action/push_back.hpp>

namespace stonks::finance {
std::optional<StrategyOrderRequest> MeanAverageStrategy::ProcessNewPrices(
    const std::vector<TimeDouble> &prices) {
  {
    const auto lock = std::lock_guard{prices_mutex_};

    prices_ |= ranges::actions::push_back(prices);
  }

  return std::nullopt;
}

std::optional<StrategyOrderRequest> MeanAverageStrategy::ProcessOrderUpdate(
    const OrderProxyToStrategyOrderUpdate &order_update) {
  return std::nullopt;
}

std::vector<TimeDouble> MeanAverageStrategy::GetAllPrices() const {
  {
    auto lock = std::lock_guard{prices_mutex_};

    return prices_;
  }
}
}  // namespace stonks::finance