#include <absl/time/time.h>
#include <fmt/core.h>
#include <gtest/gtest.h>

#include <boost/di.hpp>
#include <coroutine>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <iostream>
#include <magic_enum.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core_blsh_types.h"
#include "core_buy_low_sell_high.h"
#include "core_i_symbols_db.h"
#include "core_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "test_stk_injector.h"

namespace {
const auto kAssets = std::vector<vh::stk::core::Asset>{{"BTC"}, {"USDT"}};
const auto kSymbol = vh::stk::core::Symbol{"BTCUSDT"};
const auto kSymbolInfo = vh::stk::core::SymbolInfo{
    .symbol = kSymbol,
    .base_asset = {.asset = {"BTC"}, .min_amount = 0.001, .price_step = 0.001},
    .quote_asset = {
        .asset = {"USDT"}, .min_amount = 0.001, .price_step = 0.001}};
const auto kPrices = std::vector<vh::stk::core::SymbolPriceRecord>{
    // clang-format off
    { kSymbol,  2,  1, absl::FromUnixMillis( 0) },
    { kSymbol, 12, 11, absl::FromUnixMillis( 1) },
    { kSymbol, 22, 21, absl::FromUnixMillis( 2) },
    { kSymbol, 32, 31, absl::FromUnixMillis( 3) },
    { kSymbol, 42, 41, absl::FromUnixMillis( 4) },
    { kSymbol, 52, 51, absl::FromUnixMillis( 5) },
    { kSymbol, 62, 61, absl::FromUnixMillis( 6) },
    { kSymbol, 72, 71, absl::FromUnixMillis( 7) },
    { kSymbol, 82, 81, absl::FromUnixMillis( 8) },
    { kSymbol, 92, 91, absl::FromUnixMillis( 9) },

    { kSymbol, 92, 91, absl::FromUnixMillis(10) },
    { kSymbol, 82, 81, absl::FromUnixMillis(11) },
    { kSymbol, 72, 71, absl::FromUnixMillis(12) },
    { kSymbol, 62, 61, absl::FromUnixMillis(13) },
    { kSymbol, 52, 51, absl::FromUnixMillis(14) },
    { kSymbol, 42, 41, absl::FromUnixMillis(15) },
    { kSymbol, 32, 31, absl::FromUnixMillis(16) },
    { kSymbol, 22, 21, absl::FromUnixMillis(17) },
    { kSymbol, 12, 11, absl::FromUnixMillis(18) },
    { kSymbol,  2,  1, absl::FromUnixMillis(19) },

    { kSymbol,  2,  1, absl::FromUnixMillis(20) },
    { kSymbol, 12, 11, absl::FromUnixMillis(21) },
    { kSymbol, 22, 21, absl::FromUnixMillis(22) },
    { kSymbol, 32, 31, absl::FromUnixMillis(23) },
    { kSymbol, 42, 41, absl::FromUnixMillis(24) },
    { kSymbol, 52, 51, absl::FromUnixMillis(25) },
    { kSymbol, 62, 61, absl::FromUnixMillis(26) },
    { kSymbol, 72, 71, absl::FromUnixMillis(27) },
    { kSymbol, 82, 81, absl::FromUnixMillis(28) },
    { kSymbol, 92, 91, absl::FromUnixMillis(29) }
    // clang-format on
};

const auto kBuyLowSellHigh = []() {
  auto symbols_db = test::app::Injector().create<vh::stk::core::SymbolsDb>();

  cppcoro::sync_wait([&symbols_db]() -> cppcoro::task<> {
    co_await symbols_db.UpdateAssets(kAssets);
    co_await symbols_db.UpdateSymbolsInfo({kSymbolInfo});

    for (const auto &record : kPrices) {
      co_await symbols_db.InsertSymbolPriceRecord(record);
    }
  }());

  return vh::stk::core::BuyLowSellHigh{vh::cpp::meta::AssumeThreadSafe<
      vh::cpp::NnUp<vh::stk::core::ISymbolsDb>>(
      vh::cpp::MakeNnUp<vh::stk::core::SymbolsDb>(std::move(symbols_db)))};
}();

TEST(AppBuyLowSellHigh, CalculateNextOperations) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    const auto first_operation = vh::stk::core::blsh::Operation{
        .time = absl::FromUnixMillis(9),
        .type = vh::stk::core::blsh::OperationType::kBuy,
        .btc_buy_price = 92,
        .btc_sell_price_waiting_for = {},
        .btc_balance = 0,
        .usd_balance = 10};
    const auto next_operations =
        co_await kBuyLowSellHigh.CalculateNextOperations(
            kSymbol, 0.01, first_operation, {}, {});

    std::cout << "\nNext operations:\n";

    for (const auto &operation : next_operations) {
      std::cout << fmt::format(
          R"(time: {}
type: {}
btc_buy_price: {}
btc_sell_price_waiting_for: {}
btc_balance: {}
usd_balance: {}

)",
          absl::FormatTime(operation.time),
          magic_enum::enum_name(operation.type), operation.btc_buy_price,
          operation.btc_sell_price_waiting_for, operation.btc_balance,
          operation.usd_balance);
    }
  }());
}
}  // namespace