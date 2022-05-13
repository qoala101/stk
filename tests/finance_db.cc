#include "finance_db.h"

#include <gtest/gtest.h>

#include <chrono>
#include <optional>

#include "finance_types.h"

namespace {
const auto kTestDbFileName = "finance_db_test.db";

auto finance_db = std::optional<stonks::finance::FinanceDb>{};

TEST(FinanceDb, TablesInitialization) {
  static_cast<void>(std::remove(kTestDbFileName));
  finance_db.emplace(kTestDbFileName);

  const auto assets = finance_db->SelectAssets();
  ASSERT_TRUE(assets.has_value());
  EXPECT_FALSE(assets->empty());

  const auto symbols = finance_db->SelectSymbols();
  ASSERT_TRUE(symbols.has_value());
  EXPECT_FALSE(symbols->empty());
}

TEST(FinanceDb, InsertAndSelectSymbolPriceTicks) {
  const auto symbol_price_ticks = finance_db->SelectSymbolPriceTicks(
      std::nullopt,
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"}});
  ASSERT_TRUE(symbol_price_ticks.has_value());
  EXPECT_TRUE(symbol_price_ticks->empty());

  const auto eth_price_ticks = std::vector<stonks::finance::SymbolPriceTick>{
      stonks::finance::SymbolPriceTick{
          .symbol = stonks::finance::Symbol{.base_asset = "ETH",
                                            .quote_asset = "USDT"},
          .time = std::chrono::milliseconds{1000},
          .buy_price = 0.1,
          .sell_price = 0.01},
      stonks::finance::SymbolPriceTick{
          .symbol = stonks::finance::Symbol{.base_asset = "ETH",
                                            .quote_asset = "USDT"},
          .time = std::chrono::milliseconds{2000},
          .buy_price = 0.2,
          .sell_price = 0.02},
      stonks::finance::SymbolPriceTick{
          .symbol = stonks::finance::Symbol{.base_asset = "ETH",
                                            .quote_asset = "USDT"},
          .time = std::chrono::milliseconds{3000},
          .buy_price = 0.3,
          .sell_price = 0.03}};

  for (const auto &symbol_price_tick : eth_price_ticks) {
    const auto success = finance_db->InsertSymbolPriceTick(symbol_price_tick);
    EXPECT_TRUE(success);
  }

  const auto btc_price_ticks = std::vector<stonks::finance::SymbolPriceTick>{
      stonks::finance::SymbolPriceTick{
          .symbol = stonks::finance::Symbol{.base_asset = "BTC",
                                            .quote_asset = "USDT"},
          .time = std::chrono::milliseconds{10000},
          .buy_price = 0.01,
          .sell_price = 0.001},
      stonks::finance::SymbolPriceTick{
          .symbol = stonks::finance::Symbol{.base_asset = "BTC",
                                            .quote_asset = "USDT"},
          .time = std::chrono::milliseconds{20000},
          .buy_price = 0.02,
          .sell_price = 0.002},
      stonks::finance::SymbolPriceTick{
          .symbol = stonks::finance::Symbol{.base_asset = "BTC",
                                            .quote_asset = "USDT"},
          .time = std::chrono::milliseconds{30000},
          .buy_price = 0.03,
          .sell_price = 0.003}};

  for (const auto &symbol_price_tick : btc_price_ticks) {
    const auto success = finance_db->InsertSymbolPriceTick(symbol_price_tick);
    EXPECT_TRUE(success);
  }

  const auto all_price_ticks_received = finance_db->SelectSymbolPriceTicks();
  ASSERT_TRUE(all_price_ticks_received.has_value());
  EXPECT_EQ(all_price_ticks_received->size(),
            eth_price_ticks.size() + btc_price_ticks.size());

  const auto eth_price_ticks_received = finance_db->SelectSymbolPriceTicks(
      std::nullopt,
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"}});
  ASSERT_TRUE(eth_price_ticks_received.has_value());
  EXPECT_EQ(*eth_price_ticks_received, eth_price_ticks);

  const auto btc_price_ticks_received = finance_db->SelectSymbolPriceTicks(
      std::nullopt,
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"}});
  ASSERT_TRUE(btc_price_ticks_received.has_value());
  EXPECT_EQ(*btc_price_ticks_received, btc_price_ticks);
}

TEST(FinanceDb, SelectPeriod) {
  auto price_ticks = finance_db->SelectSymbolPriceTicks(
      stonks::finance::Period{.start_time = std::nullopt,
                              .end_time = std::nullopt},
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"},
          stonks::finance::Symbol{.base_asset = "BTC", .quote_asset = "USDT"}});
  ASSERT_TRUE(price_ticks.has_value());
  EXPECT_EQ(price_ticks->size(), 6);

  price_ticks = finance_db->SelectSymbolPriceTicks(
      stonks::finance::Period{.start_time = std::chrono::milliseconds{1001},
                              .end_time = std::nullopt},
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"}});
  ASSERT_TRUE(price_ticks.has_value());
  EXPECT_EQ(price_ticks->size(), 2);

  price_ticks = finance_db->SelectSymbolPriceTicks(
      stonks::finance::Period{.start_time = std::nullopt,
                              .end_time = std::chrono::milliseconds{2999}},
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"}});
  ASSERT_TRUE(price_ticks.has_value());
  EXPECT_EQ(price_ticks->size(), 2);

  price_ticks = finance_db->SelectSymbolPriceTicks(
      stonks::finance::Period{.start_time = std::chrono::milliseconds{1001},
                              .end_time = std::chrono::milliseconds{2999}},
      std::vector<stonks::finance::Symbol>{
          stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"}});
  ASSERT_TRUE(price_ticks.has_value());
  EXPECT_EQ(price_ticks->size(), 1);
}
}  // namespace