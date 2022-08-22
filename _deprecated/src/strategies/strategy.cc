#include "strategy.h"

#include "cpp_not_null.h"
#include <optional>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/identity.hpp>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "finance_db.h"
#include "finance_enums.h"
#include "finance_types.h"

namespace stonks {
auto Order::IsValidToBeFilled() const -> bool {
  if (states.empty()) {
    return true;
  }

  const auto status = states.back().state.GetOrderStatus();

  if (!status.has_value()) {
    return false;
  }

  return (*status == finance::OrderStatus::kNew) ||
         (*status == finance::OrderStatus::kPartiallyFilled);
}

Strategy::Strategy(Inputs required_data) : inputs_{std::move(required_data)} {}

auto Strategy::Run(const finance::SymbolName &symbol)
    -> std::vector<finance::SymbolBalanceTick> {
  inputs_.symbol = symbol;

  const auto db = finance::FinanceDb{"StrategyTestingDb.db"};

  price_ticks_ = db.SelectSymbolPriceTicks(
      std::nullopt, std::nullopt,
      std::vector<finance::SymbolName>{inputs_.symbol});
  symbols_info_ = db.SelectSymbolsInfo();

  auto partial_price_ticks = std::vector<finance::SymbolPriceTick>{};
  auto balance_ticks = std::vector<finance::SymbolBalanceTick>{};

  for (const auto &price_tick : price_ticks_) {
    auto balance_tick = finance::SymbolBalanceTick{.symbol = price_tick.symbol,
                                                   .time = price_tick.time};

    if (!balance_ticks.empty()) {
      const auto &prev_balance_tick = balance_ticks.back();
      balance_tick.base_balance = prev_balance_tick.base_balance;
      balance_tick.quote_balance = prev_balance_tick.quote_balance;
    }

    partial_price_ticks = {price_tick};
    // partial_price_ticks.emplace_back(price_tick);

    auto updated_orders = std::vector<OrderState>{};

    for (auto &order : orders_) {
      if (order.IsValidToBeFilled()) {
        const auto price = order.request.type.GetPrice();

        if (!price.has_value()) {
          continue;
        }

        // TODO(vh): need to take commission into account here
        if ((order.request.action == finance::Action::kBuy) &&
            (price_tick.buy_price <= *price)) {
          const auto order_state = OrderState{
              .time = price_tick.time,
              .order_uuid = order.request.order_uuid,
              .state = finance::OrderUpdate{
                  .order_update = finance::OrderInfo{
                      .order_status = finance::OrderStatus::kFilled,
                      .requested_amount = *order.request.amount.GetBaseAmount(),
                      .executed_amount = *order.request.amount.GetBaseAmount(),
                      .price = price_tick.buy_price,
                      .executed_quote_amount =
                          *order.request.amount.GetBaseAmount() *
                          price_tick.buy_price}}};
          order.states.emplace_back(order_state);
          updated_orders.emplace_back(order_state);

          balance_tick.base_balance += *order.request.amount.GetBaseAmount();
          balance_tick.quote_balance -=
              *order.request.amount.GetBaseAmount() * price_tick.buy_price;
          balance_tick.quote_balance +=
              *order.request.amount.GetBaseAmount() * price_tick.sell_price;

          continue;
        }

        if ((order.request.action == finance::Action::kSell) &&
            (price_tick.sell_price >= *price)) {
          const auto order_state = OrderState{
              .time = price_tick.time,
              .order_uuid = order.request.order_uuid,
              .state = finance::OrderUpdate{
                  .order_update = finance::OrderInfo{
                      .order_status = finance::OrderStatus::kFilled,
                      .requested_amount = *order.request.amount.GetBaseAmount(),
                      .executed_amount = *order.request.amount.GetBaseAmount(),
                      .price = price_tick.sell_price,
                      .executed_quote_amount =
                          *order.request.amount.GetBaseAmount() *
                          price_tick.sell_price}}};

          order.states.emplace_back(order_state);
          updated_orders.emplace_back(order_state);

          balance_tick.base_balance -= *order.request.amount.GetBaseAmount();
          balance_tick.quote_balance +=
              *order.request.amount.GetBaseAmount() * price_tick.buy_price;
          balance_tick.quote_balance -=
              *order.request.amount.GetBaseAmount() * price_tick.sell_price;

          continue;
        }
      }
    }

    ProcessEvent(Event{.time = price_tick.time,
                       .price_ticks = partial_price_ticks,
                       .updated_orders = updated_orders});

    balance_ticks.emplace_back(balance_tick);
  }

  return balance_ticks;
}

void Strategy::PostOrder(OrderRequest order_request) {
  orders_.emplace_back(Order{.request = std::move(order_request)});
}

[[nodiscard]] auto Strategy::GetSymbolInfo() const
    -> const finance::SymbolInfo & {
  return *ranges::find_if(symbols_info_, [this](const auto &symbol_info) {
    return symbol_info.symbol == inputs_.symbol;
  });
}

void Strategy::ProcessEvent(const Event & /*unused*/) {}
}  // namespace stonks