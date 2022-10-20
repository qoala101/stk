#include <absl/time/time.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <boost/di.hpp>
#include <ext/alloc_traits.h>
#include <memory>
#include <vector>

#include "app_sdb_app.h"
#include "core_types.h"
#include "cpp_smart_pointers.h"
#include "cpp_typed_struct.h"
#include "gtest/gtest_pred_impl.h"
#include "test_app_injector.h"

namespace {
auto db = stonks::cpp::Sp<stonks::app::sdb::App>{};

TEST(AppSymbolsDb, UpdateSymbolsInfo) {
  db = test::app::Injector().create<stonks::cpp::Sp<stonks::app::sdb::App>>();

  db->UpdateAssets({{"YYY"}, {"USDT"}});
  auto assets = db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0].value, "USDT");
  EXPECT_EQ(assets[1].value, "YYY");

  db->UpdateAssets({{"ETH"}, {"USDT"}});
  assets = db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0].value, "USDT");
  EXPECT_EQ(assets[1].value, "ETH");

  auto symbols_info =
      std::vector<stonks::core::SymbolInfo>{stonks::core::SymbolInfo{
          .symbol = {"ETHUSDT"},
          .base_asset = {.asset = {"ETH"}, .min_amount = 1, .price_step = 3},
          .quote_asset = {
              .asset = {"USDT"}, .min_amount = 2, .price_step = 4}}};
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  db->UpdateAssets({{"BTC"}, {"USDT"}});
  assets = db->SelectAssets();
  EXPECT_EQ(assets.size(), 2);
  EXPECT_EQ(assets[0].value, "USDT");
  EXPECT_EQ(assets[1].value, "BTC");
  EXPECT_TRUE(db->SelectSymbolsInfo().empty());

  symbols_info = std::vector<stonks::core::SymbolInfo>{stonks::core::SymbolInfo{
      .symbol = {"BTCUSDT"}, .base_asset = {"BTC"}, .quote_asset = {"USDT"}}};
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  symbols_info[0].base_asset.min_amount = 1;
  symbols_info[0].quote_asset.min_amount = 2;
  symbols_info[0].base_asset.price_step = 3;
  symbols_info[0].quote_asset.price_step = 4;
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  symbols_info[0].base_asset = {"USDT"};
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  symbols_info.emplace_back(stonks::core::SymbolInfo{
      .symbol = {"NEW_NAME"}, .base_asset = {"USDT"}, .quote_asset = {"USDT"}});
  db->UpdateSymbolsInfo(symbols_info);
  EXPECT_EQ(db->SelectSymbolsInfo(), symbols_info);

  db->InsertSymbolPriceRecord(
      stonks::core::SymbolPriceRecord{.symbol = {"BTCUSDT"}});
  EXPECT_FALSE(db->SelectSymbolPriceRecords({.symbol = {"BTCUSDT"}}).empty());

  db->UpdateAssets({{"NONE_ASSET"}});
  EXPECT_TRUE(db->SelectSymbolPriceRecords({.symbol = {"BTCUSDT"}}).empty());
}

TEST(AppSymbolsDb, InsertAndSelectSymbolPriceRecords) {
  const auto eth_usdt = stonks::core::Symbol{"ETHUSDT"};
  const auto btc_usdt = stonks::core::Symbol{"BTCUSDT"};

  db->UpdateAssets({{"BTC"}, {"ETH"}, {"USDT"}});
  db->UpdateSymbolsInfo(
      {{.symbol = {eth_usdt}, .base_asset = {"ETH"}, .quote_asset = {"USDT"}},
       {.symbol = {btc_usdt}, .base_asset = {"BTC"}, .quote_asset = {"USDT"}}});

  const auto symbol_price_records =
      db->SelectSymbolPriceRecords({.symbol = eth_usdt});
  EXPECT_TRUE(symbol_price_records.empty());

  const auto eth_price_records = std::vector<stonks::core::SymbolPriceRecord>{
      stonks::core::SymbolPriceRecord{.symbol = eth_usdt,
                                      .price = {0.1},
                                      .time = absl::FromUnixMillis(1000)},
      stonks::core::SymbolPriceRecord{.symbol = eth_usdt,
                                      .price = {0.2},
                                      .time = absl::FromUnixMillis(2000)},
      stonks::core::SymbolPriceRecord{.symbol = eth_usdt,
                                      .price = {0.3},
                                      .time = absl::FromUnixMillis(3000)}};

  for (const auto &symbol_price_record : eth_price_records) {
    db->InsertSymbolPriceRecord(symbol_price_record);
  }

  const auto btc_price_records = std::vector<stonks::core::SymbolPriceRecord>{
      stonks::core::SymbolPriceRecord{.symbol = btc_usdt,
                                      .price = {0.01},
                                      .time = absl::FromUnixMillis(10000)},
      stonks::core::SymbolPriceRecord{.symbol = btc_usdt,
                                      .price = {0.02},
                                      .time = absl::FromUnixMillis(20000)},
      stonks::core::SymbolPriceRecord{.symbol = btc_usdt,
                                      .price = {0.03},
                                      .time = absl::FromUnixMillis(30000)}};

  for (const auto &symbol_price_record : btc_price_records) {
    db->InsertSymbolPriceRecord(symbol_price_record);
  }

  const auto eth_price_records_received =
      db->SelectSymbolPriceRecords({.symbol = eth_usdt});
  EXPECT_EQ(eth_price_records_received, eth_price_records);

  const auto btc_price_records_received =
      db->SelectSymbolPriceRecords({.symbol = btc_usdt});
  EXPECT_EQ(btc_price_records_received, btc_price_records);
}

TEST(AppSymbolsDb, SelectPeriod) {
  const auto symbol = stonks::core::Symbol{"ETHUSDT"};
  auto price_records = db->SelectSymbolPriceRecords({.symbol = symbol});
  EXPECT_EQ(price_records.size(), 3);

  price_records = db->SelectSymbolPriceRecords(
      {.symbol = symbol, .start_time = absl::FromUnixMillis(1001)});
  EXPECT_EQ(price_records.size(), 2);

  price_records = db->SelectSymbolPriceRecords(
      {.symbol = symbol, .end_time = absl::FromUnixMillis(2999)});
  EXPECT_EQ(price_records.size(), 2);

  price_records =
      db->SelectSymbolPriceRecords({.symbol = symbol,
                                    .start_time = absl::FromUnixMillis(1001),
                                    .end_time = absl::FromUnixMillis(2999)});
  EXPECT_EQ(price_records.size(), 1);
}

TEST(AppSymbolsDb, SelectSymbolsWithPriceRecords) {
  const auto symbols = db->SelectSymbolsWithPriceRecords();
  EXPECT_EQ(symbols.size(), 2);
}
}  // namespace