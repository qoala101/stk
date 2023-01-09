#include "core_buy_low_sell_high.h"

#include <fmt/format.h>

#include <coroutine>
#include <gsl/assert>
#include <utility>

#include "core_blsh_types.h"
#include "core_common.h"
#include "cpp_message_exception.h"

namespace stonks::core {
namespace {
auto StartTimeFrom [[nodiscard]] (const absl::Time *time) {
  return (time == nullptr) ? absl::InfinitePast() : *time;
}

auto EndTimeFrom [[nodiscard]] (const absl::Time *time) {
  return (time == nullptr) ? absl::InfiniteFuture() : *time;
}
}  // namespace

BuyLowSellHigh::BuyLowSellHigh(cpp::NnUp<ISymbolsDb> symbols_db)
    : symbols_db_{std::move(symbols_db)}, commission_{0.001} {}

auto BuyLowSellHigh::CalculateNextOperations(
    Symbol symbol, double profit, const blsh::Operation &last_operation,
    const absl::Time *start_time, const absl::Time *end_time) const
    -> cppcoro::task<std::vector<blsh::Operation>> {
  const auto symbol_info =
      co_await symbols_db_->SelectSymbolInfo(std::move(symbol));

  if (!symbol_info.has_value()) {
    throw cpp::MessageException{
        fmt::format("Couldn't get symbol info for {}", *symbol)};
  }

  if (last_operation.type == blsh::OperationType::kBuy) {
    // User input, restricted by graph
    const auto btc_buy_price = last_operation.btc_buy_price;

    // User input, non restricted
    const auto usd_user_wants_to_spend = last_operation.usd_balance;

    const auto btc_order_amount_not_stepped =
        usd_user_wants_to_spend / btc_buy_price;
    const auto btc_order_amount =
        Ceil({.value = btc_order_amount_not_stepped,
              .precision = symbol_info->base_asset.price_step});

    const auto usd_would_be_actually_withdrawn_from_balance_to_place_order =
        btc_order_amount * btc_buy_price;
    const auto btc_would_be_actually_added_to_balance_after_order_is_executed =
        btc_order_amount * (double{1} - commission_);

    //////////////////////////

    const auto btc_sell_order_amount_not_stepped =
        last_operation.btc_balance +
        btc_would_be_actually_added_to_balance_after_order_is_executed;
    const auto btc_sell_order_amount =
        Floor({.value = btc_sell_order_amount_not_stepped,
               .precision = symbol_info->base_asset.price_step});

    const auto usd_we_spent_to_purchase_btc_we_would_sell =
        usd_would_be_actually_withdrawn_from_balance_to_place_order *
        btc_sell_order_amount /
        btc_would_be_actually_added_to_balance_after_order_is_executed;

    const auto usd_we_need_to_be_added_to_balance_to_get_profit =
        usd_we_spent_to_purchase_btc_we_would_sell * (double{1} + profit);

    const auto usd_we_need_to_receive_with_commission =
        usd_we_need_to_be_added_to_balance_to_get_profit /
        (double{1} - commission_);

    const auto btc_sell_price_not_stepped =
        usd_we_need_to_receive_with_commission / btc_sell_order_amount;
    const auto btc_sell_price =
        Ceil({.value = btc_sell_price_not_stepped,
              .precision = symbol_info->quote_asset.price_step});

    //////////////////////////

    const auto usd_would_be_actually_added_to_balance_after_order_is_executed =
        btc_sell_order_amount * btc_sell_price * (double{1} - commission_);

    co_return std::vector<blsh::Operation>{
        {.time = absl::InfiniteFuture(),
         .btc_sell_price_waiting_for = btc_sell_price,
         //  .btc_would_be_taken_from_balance = btc_sell_order_amount,
         //  .usd_would_be_added_to_balance =
         //      usd_would_be_actually_added_to_balance_after_order_is_executed,
         .btc_balance =
             last_operation.btc_balance +
             btc_would_be_actually_added_to_balance_after_order_is_executed -
             btc_sell_order_amount,
         .usd_balance =
             last_operation.usd_balance -
             usd_would_be_actually_withdrawn_from_balance_to_place_order +
             usd_would_be_actually_added_to_balance_after_order_is_executed}};
  }

  co_return std::vector<blsh::Operation>{};

  // Calculate expected operation
  // Then see if its reached
  // If its reached, update expected operation with actual time and prices etc.

  // const auto end_time_value = EndTimeFrom(end_time);
  // Expects(end_time_value > last_operation.time);

  // const auto start_time_value =
  //     std::max(StartTimeFrom(start_time), *start_time);
  // Expects(start_time_value < end_time_value);

  // const auto price_records = co_await symbols_db_->SelectSymbolPriceRecords(
  //     symbol, &start_time_value, &end_time_value, nullptr);

  // if (price_records.empty()) {
  //   co_return std::vector<blsh::Operation>{};
  // }

  // if (last_operation.type == blsh::OperationType::kSell) {
  //   co_return std::vector<blsh::Operation>{};
  // }

  // const auto waiting_for_price = 33;

  // for (const auto &record : price_records) {
  //   // record.price
  // }

  // co_return std::vector<blsh::Operation>{};
}
}  // namespace stonks::core