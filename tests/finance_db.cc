#include "finance_db.h"

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <optional>

#include "finance_types.h"
#include "stonks_db_updater_symbols_info.h"

namespace {
const auto kTestDbFileName = "finance_db_test.db";

auto finance_db = std::shared_ptr<stonks::finance::FinanceDb>{};

TEST(FinanceDb, UpdateSymbolsInfo) {
  finance_db = std::make_shared<stonks::finance::FinanceDb>(":memory:");

  finance_db->UpdateAssets({"YYY", "USDT"});
  auto assets = finance_db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0], "USDT");
  EXPECT_EQ(assets[1], "YYY");

  finance_db->UpdateAssets({"ETH", "USDT"});
  assets = finance_db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0], "USDT");
  EXPECT_EQ(assets[1], "ETH");

  auto symbols_info = std::vector<stonks::finance::SymbolInfo>{
      stonks::finance::SymbolInfo{.symbol = "ETHUSDT",
                                  .base_asset = "ETH",
                                  .quote_asset = "USDT",
                                  .min_base_amount = 1,
                                  .min_quote_amount = 2,
                                  .base_step = 3,
                                  .quote_step = 4}};
  finance_db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(finance_db->SelectSymbolsInfo(), symbols_info);

  finance_db->UpdateAssets({"BTC", "USDT"});
  assets = finance_db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0], "USDT");
  EXPECT_EQ(assets[1], "BTC");
  EXPECT_TRUE(finance_db->SelectSymbolsInfo().empty());

  symbols_info =
      std::vector<stonks::finance::SymbolInfo>{stonks::finance::SymbolInfo{
          .symbol = "BTCUSDT", .base_asset = "BTC", .quote_asset = "USDT"}};
  finance_db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(finance_db->SelectSymbolsInfo(), symbols_info);

  symbols_info[0].min_base_amount = 1;
  symbols_info[0].min_quote_amount = 2;
  symbols_info[0].base_step = 3;
  symbols_info[0].quote_step = 4;
  finance_db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(finance_db->SelectSymbolsInfo(), symbols_info);

  symbols_info[0].base_asset = "USDT";
  finance_db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(finance_db->SelectSymbolsInfo(), symbols_info);

  symbols_info.emplace_back(stonks::finance::SymbolInfo{
      .symbol = "NEW_NAME", .base_asset = "USDT", .quote_asset = "USDT"});
  finance_db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(finance_db->SelectSymbolsInfo(), symbols_info);

  finance_db->InsertSymbolPriceTick(
      stonks::finance::SymbolPriceTick{.symbol = "BTCUSDT"});
  EXPECT_FALSE(
      finance_db
          ->SelectSymbolPriceTicks(std::nullopt, std::nullopt, std::nullopt)
          .empty());

  finance_db->UpdateAssets({"NONE_ASSET"});
  EXPECT_TRUE(
      finance_db
          ->SelectSymbolPriceTicks(std::nullopt, std::nullopt, std::nullopt)
          .empty());
}

TEST(FinanceDb, TablesInitialization) {
  static_cast<void>(std::remove(kTestDbFileName));
  finance_db = std::make_shared<stonks::finance::FinanceDb>(kTestDbFileName);
  static_cast<void>(stonks::DbUpdaterSymbolsInfo{finance_db});

  const auto assets = finance_db->SelectAssets();
  EXPECT_FALSE(assets.empty());

  const auto symbols = finance_db->SelectSymbols();
  EXPECT_FALSE(symbols.empty());
}

TEST(FinanceDb, InsertAndSelectSymbolPriceTicks) {
  const auto symbol_price_ticks = finance_db->SelectSymbolPriceTicks(
      std::nullopt, std::nullopt,
      std::vector<stonks::finance::SymbolName>{"ETHUSDT"});
  EXPECT_TRUE(symbol_price_ticks.empty());

  const auto eth_price_ticks = std::vector<stonks::finance::SymbolPriceTick>{
      stonks::finance::SymbolPriceTick{.symbol = "ETHUSDT",
                                       .time = std::chrono::milliseconds{1000},
                                       .buy_price = 0.1,
                                       .sell_price = 0.01},
      stonks::finance::SymbolPriceTick{.symbol = "ETHUSDT",
                                       .time = std::chrono::milliseconds{2000},
                                       .buy_price = 0.2,
                                       .sell_price = 0.02},
      stonks::finance::SymbolPriceTick{.symbol = "ETHUSDT",
                                       .time = std::chrono::milliseconds{3000},
                                       .buy_price = 0.3,
                                       .sell_price = 0.03}};

  for (const auto &symbol_price_tick : eth_price_ticks) {
    finance_db->InsertSymbolPriceTick(symbol_price_tick);
  }

  const auto btc_price_ticks = std::vector<stonks::finance::SymbolPriceTick>{
      stonks::finance::SymbolPriceTick{.symbol = "BTCUSDT",
                                       .time = std::chrono::milliseconds{10000},
                                       .buy_price = 0.01,
                                       .sell_price = 0.001},
      stonks::finance::SymbolPriceTick{.symbol = "BTCUSDT",
                                       .time = std::chrono::milliseconds{20000},
                                       .buy_price = 0.02,
                                       .sell_price = 0.002},
      stonks::finance::SymbolPriceTick{.symbol = "BTCUSDT",
                                       .time = std::chrono::milliseconds{30000},
                                       .buy_price = 0.03,
                                       .sell_price = 0.003}};

  for (const auto &symbol_price_tick : btc_price_ticks) {
    finance_db->InsertSymbolPriceTick(symbol_price_tick);
  }

  // const auto all_price_ticks_received = finance_db->SelectSymbolPriceTicks(
  //     std::nullopt, std::nullopt, std::nullopt);
  // EXPECT_EQ(all_price_ticks_received.size(),
  //           eth_price_ticks.size() + btc_price_ticks.size());

  // const auto eth_price_ticks_received = finance_db->SelectSymbolPriceTicks(
  //     std::nullopt, std::nullopt,
  //     std::vector<stonks::finance::SymbolName>{"ETHUSDT"});
  // EXPECT_EQ(eth_price_ticks_received, eth_price_ticks);

  // const auto btc_price_ticks_received = finance_db->SelectSymbolPriceTicks(
  //     std::nullopt, std::nullopt,
  //     std::vector<stonks::finance::SymbolName>{"BTCUSDT"});
  // EXPECT_EQ(btc_price_ticks_received, btc_price_ticks);
}

// TEST(FinanceDb, SelectPeriod) {
//   auto price_ticks = finance_db->SelectSymbolPriceTicks(
//       std::nullopt,
//       stonks::finance::Period{.start_time = std::nullopt,
//                               .end_time = std::nullopt},
//       std::vector<stonks::finance::SymbolName>{"ETHUSDT", "BTCUSDT"});
//   EXPECT_EQ(price_ticks.size(), 6);

//   price_ticks = finance_db->SelectSymbolPriceTicks(
//       std::nullopt,
//       stonks::finance::Period{.start_time = std::chrono::milliseconds{1001},
//                               .end_time = std::nullopt},
//       std::vector<stonks::finance::SymbolName>{"ETHUSDT"});
//   EXPECT_EQ(price_ticks.size(), 2);

//   price_ticks = finance_db->SelectSymbolPriceTicks(
//       std::nullopt,
//       stonks::finance::Period{.start_time = std::nullopt,
//                               .end_time = std::chrono::milliseconds{2999}},
//       std::vector<stonks::finance::SymbolName>{"ETHUSDT"});
//   EXPECT_EQ(price_ticks.size(), 2);

//   price_ticks = finance_db->SelectSymbolPriceTicks(
//       std::nullopt,
//       stonks::finance::Period{.start_time = std::chrono::milliseconds{1001},
//                               .end_time = std::chrono::milliseconds{2999}},
//       std::vector<stonks::finance::SymbolName>{"ETHUSDT"});
//   EXPECT_EQ(price_ticks.size(), 1);
// }

// TEST(FinanceDb, SelectLimit) {
//   auto price_ticks =
//       finance_db->SelectSymbolPriceTicks(4, std::nullopt, std::nullopt);
//   EXPECT_EQ(price_ticks.size(), 4);
// }
}  // namespace