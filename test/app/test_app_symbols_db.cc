#include <gtest/gtest.h>

#include <optional>

#include "cpp_not_null.h"
#include "app_sdb_app.h"

namespace {
const auto kTestDbFileName = "stonks_db_test.db";

auto db = cpp::Sp<stonks::app::sdb::App>{};

TEST(StonksDb, UpdateSymbolsInfo) {
  db = cpp::MakeSp<stonks::Db>(stonks::sqlite::Factory{}, ":memory:");

  db->UpdateAssets({"YYY", "USDT"});
  auto assets = db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0], "USDT");
  EXPECT_EQ(assets[1], "YYY");

  db->UpdateAssets({"ETH", "USDT"});
  assets = db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0], "USDT");
  EXPECT_EQ(assets[1], "ETH");

  auto symbols_info =
      std::vector<stonks::SymbolInfo>{stonks::SymbolInfo{.symbol = "ETHUSDT",
                                                         .base_asset = "ETH",
                                                         .quote_asset = "USDT",
                                                         .min_base_amount = 1,
                                                         .min_quote_amount = 2,
                                                         .base_step = 3,
                                                         .quote_step = 4}};
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  db->UpdateAssets({"BTC", "USDT"});
  assets = db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0], "USDT");
  EXPECT_EQ(assets[1], "BTC");
  EXPECT_TRUE(db->SelectSymbolsInfo().empty());

  symbols_info = std::vector<stonks::SymbolInfo>{stonks::SymbolInfo{
      .symbol = "BTCUSDT", .base_asset = "BTC", .quote_asset = "USDT"}};
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  symbols_info[0].min_base_amount = 1;
  symbols_info[0].min_quote_amount = 2;
  symbols_info[0].base_step = 3;
  symbols_info[0].quote_step = 4;
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  symbols_info[0].base_asset = "USDT";
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  symbols_info.emplace_back(stonks::SymbolInfo{
      .symbol = "NEW_NAME", .base_asset = "USDT", .quote_asset = "USDT"});
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  db->InsertSymbolPriceTick(stonks::SymbolPriceTick{.symbol = "BTCUSDT"});
  EXPECT_FALSE(db->SelectSymbolPriceTicks(nullptr, nullptr, nullptr).empty());

  db->UpdateAssets({"NONE_ASSET"});
  EXPECT_TRUE(db->SelectSymbolPriceTicks(nullptr, nullptr, nullptr).empty());
}

TEST(StonksDb, TablesInitialization) {
  std::ignore = std::remove(kTestDbFileName);
  db = cpp::MakeSp<stonks::Db>(stonks::sqlite::Factory{}, kTestDbFileName);
  std::ignore = stonks::DbUpdaterSymbolsInfo{db};

  const auto assets = db->SelectAssets();
  EXPECT_FALSE(assets.empty());

  const auto symbols = db->SelectSymbols();
  EXPECT_FALSE(symbols.empty());
}

TEST(StonksDb, InsertAndSelectSymbolPriceTicks) {
  const auto eth_usdt = stonks::SymbolName{"ETHUSDT"};
  const auto btc_usdt = stonks::SymbolName{"BTCUSDT"};
  const auto symbol_price_ticks =
      db->SelectSymbolPriceTicks(&eth_usdt, nullptr, nullptr);
  EXPECT_TRUE(symbol_price_ticks.empty());

  const auto eth_price_ticks = std::vector<stonks::SymbolPriceTick>{
      stonks::SymbolPriceTick{.symbol = eth_usdt,
                              .time = absl::Time{1000},
                              .buy_price = 0.1,
                              .sell_price = 0.01},
      stonks::SymbolPriceTick{.symbol = eth_usdt,
                              .time = absl::Time{2000},
                              .buy_price = 0.2,
                              .sell_price = 0.02},
      stonks::SymbolPriceTick{.symbol = eth_usdt,
                              .time = absl::Time{3000},
                              .buy_price = 0.3,
                              .sell_price = 0.03}};

  for (const auto &symbol_price_tick : eth_price_ticks) {
    db->InsertSymbolPriceTick(symbol_price_tick);
  }

  const auto btc_price_ticks = std::vector<stonks::SymbolPriceTick>{
      stonks::SymbolPriceTick{.symbol = btc_usdt,
                              .time = absl::Time{10000},
                              .buy_price = 0.01,
                              .sell_price = 0.001},
      stonks::SymbolPriceTick{.symbol = btc_usdt,
                              .time = absl::Time{20000},
                              .buy_price = 0.02,
                              .sell_price = 0.002},
      stonks::SymbolPriceTick{.symbol = btc_usdt,
                              .time = absl::Time{30000},
                              .buy_price = 0.03,
                              .sell_price = 0.003}};

  for (const auto &symbol_price_tick : btc_price_ticks) {
    db->InsertSymbolPriceTick(symbol_price_tick);
  }

  const auto all_price_ticks_received =
      db->SelectSymbolPriceTicks(nullptr, nullptr, nullptr);
  EXPECT_EQ(all_price_ticks_received.size(),
            eth_price_ticks.size() + btc_price_ticks.size());

  const auto eth_price_ticks_received =
      db->SelectSymbolPriceTicks(&eth_usdt, nullptr, nullptr);
  EXPECT_EQ(eth_price_ticks_received, eth_price_ticks);

  const auto btc_price_ticks_received =
      db->SelectSymbolPriceTicks(&btc_usdt, nullptr, nullptr);
  EXPECT_EQ(btc_price_ticks_received, btc_price_ticks);
}

TEST(StonksDb, SelectPeriod) {
  auto period =
      stonks::Period{.start_time = std::nullopt, .end_time = std::nullopt};
  auto price_ticks = db->SelectSymbolPriceTicks(nullptr, &period, nullptr);
  EXPECT_EQ(price_ticks.size(), 6);

  period = stonks::Period{.start_time = cpp::MakeOpt<absl::Time>(1001),
                          .end_time = std::nullopt};
  auto symbol = stonks::SymbolName{"ETHUSDT"};
  price_ticks = db->SelectSymbolPriceTicks(&symbol, &period, nullptr);
  EXPECT_EQ(price_ticks.size(), 2);

  period = stonks::Period{.start_time = std::nullopt,
                          .end_time = cpp::MakeOpt<absl::Time>(2999)};
  price_ticks = db->SelectSymbolPriceTicks(&symbol, &period, nullptr);
  EXPECT_EQ(price_ticks.size(), 2);

  period = stonks::Period{.start_time = cpp::MakeOpt<absl::Time>(1001),
                          .end_time = cpp::MakeOpt<absl::Time>(2999)};
  price_ticks = db->SelectSymbolPriceTicks(&symbol, &period, nullptr);
  EXPECT_EQ(price_ticks.size(), 1);
}

TEST(StonksDb, SelectLimit) {
  const auto limit = 4;
  auto price_ticks = db->SelectSymbolPriceTicks(nullptr, nullptr, &limit);
  EXPECT_EQ(price_ticks.size(), 4);
}
}  // namespace