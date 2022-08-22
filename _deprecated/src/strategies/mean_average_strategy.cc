#include "mean_average_strategy.h"

#include <assert.h>
#include <bits/exception.h>
#include <fmt/format.h>
#include <math.h>
#include <spdlog/spdlog.h>

#include <boost/uuid/detail/uuid_x86.ipp>
#include <chrono>
#include <compare>
#include <iterator>
#include "cpp_not_null.h"
#include <range/v3/action/action.hpp>
#include <range/v3/action/push_back.hpp>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/functional/identity.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/view.hpp>
#include <ratio>
#include <utility>

#include "finance_enums.h"
#include "utils.h"

namespace stonks::finance {
namespace {
double CeilValueToPercision(const double value, const double percision) {
  const auto num_percisions = std::ceil(value / percision);
  return num_percisions * percision;
}

double FloorValueToPercision(const double value, const double percision) {
  const auto num_percisions = std::floor(value / percision);
  return num_percisions * percision;
}

void DropItemsBeforeMax(std::vector<TimeDouble> &data) {
  if (data.size() < 2) {
    return;
  }

  const auto last_max_item = ranges::max_element(
      data, [](const TimeDouble &left, const TimeDouble &right) {
        return left.value < right.value;
      });

  if (last_max_item != data.begin()) {
    data = std::vector<TimeDouble>{last_max_item, data.end()};
  }
}

void DropItemsOlderThanFromLast(std::vector<TimeDouble> &data,
                                std::chrono::milliseconds period) {
  if (data.size() < 2) {
    return;
  }

  const auto old_time = data.back().time - period;
  const auto item_is_older = [old_time](const TimeDouble &item) {
    return item.time < old_time;
  };

  data = data | ranges::views::drop_while(item_is_older) | ranges::to_vector;
}
}  // namespace

MeanAverageStrategy::MeanAverageStrategy(double base_precision,
                                         double price_precision,
                                         double comission, double profit)
    : base_precision_{base_precision},
      price_precision_{price_precision},
      comission_{comission},
      profit_{profit},
      sell_order_price_{0},
      sell_order_amount_{0} {}

std::optional<StrategyOrderRequest> MeanAverageStrategy::ProcessNewPrices(
    const std::vector<TimeDouble> &prices) {
  {
    const auto lock = std::lock_guard{prices_mutex_};

    prices_ |= ranges::actions::push_back(prices);
    DropItemsBeforeMax(prices_);
    DropItemsOlderThanFromLast(prices_, std::chrono::hours{3});
  }

  {
    const auto lock = std::lock_guard{order_uuids_mutex_};

    if (const auto any_order_is_active =
            !buy_order_uuid_.is_nil() || !sell_order_uuid_.is_nil()) {
      return std::nullopt;
    }

    if (prices_.size() < 2) {
      return std::nullopt;
    }

    const auto min_item = ranges::min_element(
        prices_, [](const TimeDouble &left, const TimeDouble &right) {
          return left.value < right.value;
        });

    assert(min_item != prices_.end());

    const auto max_price = prices_.front().value;

    spdlog::info("MEAN: min {}, max {}, curr {}", min_item->value, max_price,
                 std::prev(prices_.end())->value);

    if (const auto last_price_is_not_min =
            std::prev(prices_.end()) != min_item) {
      spdlog::info("MEAN: last_price_is_not_min");
      return std::nullopt;
    }

    const auto min_price = prices_.back().value;

    // First trade low price at which base is bought.
    const auto base_buy_price = min_price;
    const auto want_to_spend_quote_amount = double{15};
    const auto base_amount_for_want_to_spend_quote_amount =
        want_to_spend_quote_amount / base_buy_price;
    const auto base_amount_to_buy = CeilValueToPercision(
        base_amount_for_want_to_spend_quote_amount, base_precision_);
    // Quote amount withdrawn from account for the first trade.
    const auto quote_amount_to_spend = base_amount_to_buy * base_buy_price;
    // Base amount to be added to account after the first trade.
    const auto base_amount_received =
        base_amount_to_buy * (double{1} - comission_);
    const auto want_to_spend_base_amount = base_amount_received;
    // Base amount withdrawn from account for the second trade.
    const auto base_amount_to_sell =
        FloorValueToPercision(want_to_spend_base_amount, base_precision_);
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
        CeilValueToPercision(sell_base_price, price_precision_);

    const auto buy_order_price = base_buy_price;
    const auto buy_order_amount = base_amount_to_buy;

    const auto sell_order_price = normalized_sell_base_price;
    const auto sell_order_amount = base_amount_to_sell;

    spdlog::info(
        "MEAN: buy_order_price {}, buy_order_amount {}, sell_order_price {}, "
        "sell_order_amount {}",
        buy_order_price, buy_order_amount, sell_order_price, sell_order_amount);

    if (const auto max_price_is_small = max_price < sell_order_price) {
      spdlog::info("MEAN: max_price_is_small");
      return std::nullopt;
    }

    buy_order_uuid_ = utils::GenerateUuid();
    sell_order_price_ = sell_order_price;
    sell_order_amount_ = sell_order_amount;

    spdlog::info("MEAN: buy order {}",
                 utils::ConvertUuidToString(buy_order_uuid_));

    return StrategyOrderRequest{
        .order_uuid = buy_order_uuid_,
        .buy_or_sell = BuyOrSell::kBuy,
        .amount =
            Amount{.amount = BaseAmount{.base_amount = base_amount_to_buy}},
        .order_type =
            OrderType{.order_type = LimitOrderType{.price = base_buy_price}},
        .strategy_data =
            StrategyData{.strategy_data = MeanAverageStrategyData{}}};
  }
}

std::optional<StrategyOrderRequest> MeanAverageStrategy::ProcessOrderUpdate(
    const OrderProxyToStrategyOrderUpdate &order_update) {
  {
    const auto lock = std::lock_guard{order_uuids_mutex_};

    if (order_update.order_uuid == buy_order_uuid_) {
      const auto order_status = order_update.order_update.GetOrderStatus();

      if (const auto order_is_not_filled =
              !order_status.has_value() ||
              (*order_status != OrderStatus::kFilled)) {
        return std::nullopt;
      }

      buy_order_uuid_ = boost::uuids::uuid{};
      sell_order_uuid_ = utils::GenerateUuid();

      return StrategyOrderRequest{
          .order_uuid = sell_order_uuid_,
          .buy_or_sell = BuyOrSell::kSell,
          .amount =
              Amount{.amount = BaseAmount{.base_amount = sell_order_amount_}},
          .order_type =
              OrderType{.order_type =
                            LimitOrderType{.price = sell_order_price_}},
          .strategy_data =
              StrategyData{.strategy_data = MeanAverageStrategyData{}}};
    }

    if (order_update.order_uuid == sell_order_uuid_) {
      const auto order_status = order_update.order_update.GetOrderStatus();

      if (const auto order_is_not_filled =
              !order_status.has_value() ||
              (*order_status != OrderStatus::kFilled)) {
        return std::nullopt;
      }

      sell_order_uuid_ = boost::uuids::uuid{};
    }
  }

  return std::nullopt;
}

std::vector<TimeDouble> MeanAverageStrategy::GetAllPrices() const {
  {
    auto lock = std::lock_guard{prices_mutex_};

    return prices_;
  }
}
}  // namespace stonks::finance