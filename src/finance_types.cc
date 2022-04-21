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
  return std::visit(
      [](const auto &variant) -> std::optional<OrderStatus> {
        using T = std::decay_t<decltype(variant)>;

        if constexpr (std::is_same_v<T, finance::OrderInfo>) {
          return variant.order_status;
        }

        return std::nullopt;
      },
      order_update);
}

std::optional<double> Amount::GetAmount() const { return std::nullopt; }

std::optional<double> OrderType::GetPrice() const { return std::nullopt; }

std::optional<OrderInfo> Order::GetLastFilledOrderInfo() const {
  if (order_updates.empty()) {
    return std::nullopt;
  }

  const auto order_is_filled = [](const OrderProxyOrderUpdate &order_update) {
    return order_update.order_update.GetOrderStatus().value_or(
               OrderStatus::kInvalid) == OrderStatus::kFilled;
  };
  const auto filled_order_update = std::find_if(
      order_updates.rbegin(), order_updates.rend(), order_is_filled);

  if (filled_order_update == order_updates.rend()) {
    return std::nullopt;
  }

  return std::get<OrderInfo>(filled_order_update->order_update.order_update);
}
};  // namespace stonks::finance