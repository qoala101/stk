#include "v1_strategy.h"

#include <absl/base/macros.h>
#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <boost/uuid/detail/uuid_x86.ipp>
#include <chrono>
#include <cmath>
#include <compare>
#include <iterator>
#include <memory>
#include <optional>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/view.hpp>
#include <ratio>
#include <utility>

#include "finance_enums.h"
#include "strategy.h"
#include "utils.h"

namespace stonks {
namespace {
double CeilValueToPercision(const double value, const double percision) {
  const auto num_percisions = std::ceil(value / percision);
  return num_percisions * percision;
}

double FloorValueToPercision(const double value, const double percision) {
  const auto num_percisions = std::floor(value / percision);
  return num_percisions * percision;
}

void DropItemsBeforeMaxBuyPrice(std::vector<finance::SymbolPriceTick> &data) {
  if (data.size() < 2) {
    return;
  }

  const auto last_max_item =
      ranges::max_element(data, [](const auto &left, const auto &right) {
        return left.buy_price < right.buy_price;
      });

  if (last_max_item != data.begin()) {
    data = std::vector<finance::SymbolPriceTick>{last_max_item, data.end()};
  }
}

void DropItemsOlderThanFromLast(std::vector<finance::TimeDouble> &data,
                                std::chrono::milliseconds period) {
  if (data.size() < 2) {
    return;
  }

  const auto old_time = data.back().time - period;
  const auto item_is_older = [old_time](const finance::TimeDouble &item) {
    return item.time < old_time;
  };

  data = data | ranges::views::drop_while(item_is_older) | ranges::to_vector;
}
}  // namespace

V1Strategy::V1Strategy()
    : Strategy(Inputs{/*.symbol = "ETHBULLBUSD",*/ .price_ticks = true}) {}

void V1Strategy::ProcessEvent(const Event &event) {
  for (const auto &order_update : event.updated_orders) {
    {
      const auto lock = std::lock_guard{order_uuids_mutex_};

      if (order_update.order_uuid == buy_order_uuid_) {
        const auto order_status = order_update.state.GetOrderStatus();

        if (const auto order_is_not_filled =
                !order_status.has_value() ||
                (*order_status != finance::OrderStatus::kFilled)) {
          return;
        }

        buy_order_uuid_ = boost::uuids::uuid{};
        sell_order_uuid_ = utils::GenerateUuid();

        spdlog::info(
            "MEAN: sell_order_price "
            "{}, "
            "sell_order_amount {}",
            sell_order_price_, sell_order_amount_);

        PostOrder(OrderRequest{
            .order_uuid = sell_order_uuid_,
            .action = finance::Action::kSell,
            .amount =
                finance::Amount{
                    .amount =
                        finance::BaseAmount{.base_amount = sell_order_amount_}},
            .type = finance::OrderType{.order_type = finance::LimitOrderType{
                                           .price = sell_order_price_}}});
        return;
      }

      if (order_update.order_uuid == sell_order_uuid_) {
        const auto order_status = order_update.state.GetOrderStatus();

        if (const auto order_is_not_filled =
                !order_status.has_value() ||
                (*order_status != finance::OrderStatus::kFilled)) {
          return;
        }

        sell_order_uuid_ = boost::uuids::uuid{};
      }
    }
  }

  const auto &prices = event.price_ticks;

  {
    const auto lock = std::lock_guard{prices_mutex_};

    prices_.emplace_back(prices.back());
    DropItemsBeforeMaxBuyPrice(prices_);
  }

  {
    const auto lock = std::lock_guard{order_uuids_mutex_};

    if (const auto any_order_is_active =
            !buy_order_uuid_.is_nil() || !sell_order_uuid_.is_nil()) {
      return;
    }

    if (prices_.size() < 2) {
      return;
    }

    const auto min_item =
        ranges::min_element(prices_, [](const auto &left, const auto &right) {
          return left.buy_price < right.buy_price;
        });
    ABSL_ASSERT(min_item != prices_.end());

    const auto max_price = prices_.front().buy_price;

    // spdlog::info("MEAN: min {}, max {}, curr {}", min_item->buy_price,
    //              max_price, std::prev(prices_.end())->buy_price);

    if (const auto last_price_is_not_min =
            std::prev(prices_.end()) != min_item) {
      // spdlog::info("MEAN: last_price_is_not_min");
      return;
    }

    const auto &symbol_info = GetSymbolInfo();
    const auto min_price = prices_.back().buy_price;

    // First trade low price at which base is bought.
    const auto base_buy_price = min_price;
    const auto want_to_spend_quote_amount = double{15};
    const auto base_amount_for_want_to_spend_quote_amount =
        want_to_spend_quote_amount / base_buy_price;
    const auto base_amount_to_buy = CeilValueToPercision(
        base_amount_for_want_to_spend_quote_amount, symbol_info.base_step);
    // Quote amount withdrawn from account for the first trade.
    const auto quote_amount_to_spend = base_amount_to_buy * base_buy_price;
    // Base amount to be added to account after the first trade.
    const auto base_amount_received =
        base_amount_to_buy * (double{1} - comission_);
    const auto want_to_spend_base_amount = base_amount_received;
    // Base amount withdrawn from account for the second trade.
    const auto base_amount_to_sell =
        FloorValueToPercision(want_to_spend_base_amount, symbol_info.base_step);
    const auto quote_amount_spent_on_base_to_sell =
        quote_amount_to_spend * base_amount_to_sell / base_amount_received;
    // Quote amount to be added to account after the first trade.
    const auto quote_amount_spent_with_profit =
        quote_amount_spent_on_base_to_sell * (double{1} + profit_);
    const auto quote_amount_to_sell_base_for =
        quote_amount_spent_with_profit / (double{1} - comission_);
    const auto sell_base_price =
        quote_amount_to_sell_base_for / base_amount_to_sell;
    // Second trade high price at which base is sold.
    const auto normalized_sell_base_price =
        CeilValueToPercision(sell_base_price, symbol_info.quote_step);

    const auto buy_order_price = base_buy_price;
    const auto buy_order_amount = base_amount_to_buy;

    const auto sell_order_price = normalized_sell_base_price;
    const auto sell_order_amount = base_amount_to_sell;

    // spdlog::info(
    //     "MEAN: buy_order_price {}, buy_order_amount {}, sell_order_price {},
    //     " "sell_order_amount {}", buy_order_price, buy_order_amount,
    //     sell_order_price, sell_order_amount);

    if (const auto max_price_is_small = max_price < sell_order_price) {
      // spdlog::info("MEAN: max_price_is_small");
      return;
    }

    buy_order_uuid_ = utils::GenerateUuid();
    sell_order_price_ = sell_order_price;
    sell_order_amount_ = sell_order_amount;

    spdlog::info("MEAN: buy order {}",
                 utils::ConvertUuidToString(buy_order_uuid_));

    PostOrder(OrderRequest{
        .time = event.time,
        .order_uuid = buy_order_uuid_,
        .action = finance::BuyOrSell::kBuy,
        .amount = finance::Amount{.amount =
                                      finance::BaseAmount{
                                          .base_amount = base_amount_to_buy}},
        .type = finance::OrderType{
            .order_type = finance::LimitOrderType{.price = base_buy_price}}});
  }
}
}  // namespace stonks