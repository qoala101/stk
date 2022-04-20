#include "finance_types.h"

#include <tuple>

namespace stonks::finance {
bool operator==(const Candle &left, const Candle &right) {
  const auto equal = std::tie(left.open_time, left.close_time) ==
                     std::tie(right.open_time, right.close_time);

  if (!equal) {
    return equal;
  }

  return left.data.value_or(Candle::Data{}) ==
         right.data.value_or(Candle::Data{});
}

std::partial_ordering operator<=>(const Candle &left, const Candle &right) {
  const auto ordering = std::tie(left.open_time, left.close_time) <=>
                        std::tie(right.open_time, right.close_time);

  if (ordering != std::partial_ordering::equivalent) {
    return ordering;
  }

  return left.data.value_or(Candle::Data{}) <=>
         right.data.value_or(Candle::Data{});
}

std::optional<std::string> StrategyData::GetName() const {
  return std::nullopt;
}

std::optional<std::string> OrderUpdate::GetErrorMessage() const {
  return std::nullopt;
}

std::optional<OrderStatus> OrderUpdate::GetOrderStatus() const {
  return std::nullopt;
}

std::optional<double> Amount::GetAmount() const { return std::nullopt; }

std::optional<double> OrderType::GetPrice() const { return std::nullopt; }
};  // namespace stonks::finance