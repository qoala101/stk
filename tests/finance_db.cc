#include "finance_db.h"

#include <gtest/gtest.h>

#include <chrono>
#include <optional>

#include "finance_types.h"

namespace {
const auto kTestDbFileName = "finance_db_test.db";

auto finance_db = std::optional<stonks::finance::FinanceDb>{};

TEST(FinanceDb, TablesInitialization) {
  std::remove(kTestDbFileName);
  finance_db.emplace(kTestDbFileName);

  const auto assets = finance_db->SelectAssets();
  ASSERT_TRUE(assets.has_value());
  EXPECT_FALSE(assets->empty());

  const auto symbols = finance_db->SelectSymbols();
  ASSERT_TRUE(symbols.has_value());
  EXPECT_FALSE(symbols->empty());

  const auto symbol_ticks = finance_db->SelectSymbolBookTicks(
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"});
  ASSERT_TRUE(symbol_ticks.has_value());
  EXPECT_TRUE(symbols->empty());
}

TEST(FinanceDb, InsertAndSelectSymbolPrices) {
  const auto eth_prices = std::vector<stonks::finance::TimeDouble>{
      stonks::finance::TimeDouble{.time = std::chrono::milliseconds{1000},
                                  .value = 0.1},
      stonks::finance::TimeDouble{.time = std::chrono::milliseconds{2000},
                                  .value = 0.2},
      stonks::finance::TimeDouble{.time = std::chrono::milliseconds{3000},
                                  .value = 0.3}};
  const auto btc_prices = std::vector<stonks::finance::TimeDouble>{
      stonks::finance::TimeDouble{.time = std::chrono::milliseconds{10000},
                                  .value = double{1}},
      stonks::finance::TimeDouble{.time = std::chrono::milliseconds{20000},
                                  .value = double{2}},
      stonks::finance::TimeDouble{.time = std::chrono::milliseconds{30000},
                                  .value = double{3}}};

  const auto success = finance_db->InsertSymbolsPrices(
      {stonks::finance::SymbolPrices{
           .symbol = stonks::finance::Symbol{.base_asset = "ETH",
                                             .quote_asset = "USDT"},
           .prices = eth_prices},

       stonks::finance::SymbolPrices{
           .symbol = stonks::finance::Symbol{.base_asset = "BTC",
                                             .quote_asset = "USDT"},
           .prices = btc_prices}});
  EXPECT_TRUE(success);

  const auto eth_prices_received = finance_db->SelectSymbolPrices(
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"});
  ASSERT_TRUE(eth_prices_received.has_value());
  EXPECT_EQ(*eth_prices_received, eth_prices);

  const auto btc_prices_received = finance_db->SelectSymbolPrices(
      stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"});
  ASSERT_TRUE(btc_prices_received.has_value());
  EXPECT_EQ(*btc_prices_received, btc_prices);
}
}  // namespace