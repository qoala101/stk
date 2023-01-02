#include <absl/time/time.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <boost/di.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <ext/alloc_traits.h>
#include <memory>
#include <vector>

#include "core_symbols_db.h"
#include "core_types.h"
#include "cpp_smart_pointers.h"
#include "cpp_typed_struct.h"
#include "gtest/gtest_pred_impl.h"
#include "test_app_injector.h"

namespace {
auto symbols_db = []() {
  return test::app::Injector().create<stonks::core::SymbolsDb>();
}();

TEST(AppSymbolsDb, UpdateSymbolsInfo) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    co_await symbols_db.UpdateAssets({{"YYY"}, {"USDT"}});
    auto assets = co_await symbols_db.SelectAssets();
    EXPECT_EQ(assets.size(), 2);
    EXPECT_EQ(*assets[0], "USDT");
    EXPECT_EQ(*assets[1], "YYY");

    co_await symbols_db.UpdateAssets({{"ETH"}, {"USDT"}});
    assets = co_await symbols_db.SelectAssets();
    EXPECT_EQ(assets.size(), 2);
    EXPECT_EQ(*assets[0], "USDT");
    EXPECT_EQ(*assets[1], "ETH");

    auto symbols_info =
        std::vector<stonks::core::SymbolInfo>{stonks::core::SymbolInfo{
            .symbol = {"ETHUSDT"},
            .base_asset = {.asset = {"ETH"}, .min_amount = 1, .price_step = 3},
            .quote_asset = {
                .asset = {"USDT"}, .min_amount = 2, .price_step = 4}}};
    co_await symbols_db.UpdateSymbolsInfo(symbols_info);
    EXPECT_EQ(co_await symbols_db.SelectSymbolsInfo(), symbols_info);

    co_await symbols_db.UpdateAssets({{"BTC"}, {"USDT"}});
    assets = co_await symbols_db.SelectAssets();
    EXPECT_EQ(assets.size(), 2);
    EXPECT_EQ(*assets[0], "USDT");
    EXPECT_EQ(*assets[1], "BTC");
    EXPECT_TRUE((co_await symbols_db.SelectSymbolsInfo()).empty());

    symbols_info = std::vector<stonks::core::SymbolInfo>{
        stonks::core::SymbolInfo{.symbol = {"BTCUSDT"},
                                 .base_asset = {"BTC"},
                                 .quote_asset = {"USDT"}}};
    co_await symbols_db.UpdateSymbolsInfo(symbols_info);
    EXPECT_EQ(co_await symbols_db.SelectSymbolsInfo(), symbols_info);

    symbols_info[0].base_asset.min_amount = 1;
    symbols_info[0].quote_asset.min_amount = 2;
    symbols_info[0].base_asset.price_step = 3;
    symbols_info[0].quote_asset.price_step = 4;
    co_await symbols_db.UpdateSymbolsInfo(symbols_info);
    EXPECT_EQ(co_await symbols_db.SelectSymbolsInfo(), symbols_info);

    symbols_info[0].base_asset = {"USDT"};
    co_await symbols_db.UpdateSymbolsInfo(symbols_info);
    EXPECT_EQ(co_await symbols_db.SelectSymbolsInfo(), symbols_info);

    symbols_info.emplace_back(
        stonks::core::SymbolInfo{.symbol = {"NEW_NAME"},
                                 .base_asset = {"USDT"},
                                 .quote_asset = {"USDT"}});
    co_await symbols_db.UpdateSymbolsInfo(symbols_info);
    EXPECT_EQ(co_await symbols_db.SelectSymbolsInfo(), symbols_info);

    co_await symbols_db.InsertSymbolPriceRecord(
        stonks::core::SymbolPriceRecord{.symbol = {"BTCUSDT"}});
    EXPECT_FALSE((co_await symbols_db.SelectSymbolPriceRecords(
                      {"BTCUSDT"}, nullptr, nullptr, nullptr))
                     .empty());

    co_await symbols_db.UpdateAssets({{"NONE_ASSET"}});
    EXPECT_TRUE((co_await symbols_db.SelectSymbolPriceRecords(
                     {"BTCUSDT"}, nullptr, nullptr, nullptr))
                    .empty());
  }());
}

TEST(AppSymbolsDb, InsertAndSelectSymbolPriceRecords) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    const auto eth_usdt = stonks::core::Symbol{"ETHUSDT"};
    const auto btc_usdt = stonks::core::Symbol{"BTCUSDT"};

    co_await symbols_db.UpdateAssets({{"BTC"}, {"ETH"}, {"USDT"}});
    co_await symbols_db.UpdateSymbolsInfo(
        {{.symbol = {eth_usdt}, .base_asset = {"ETH"}, .quote_asset = {"USDT"}},
         {.symbol = {btc_usdt},
          .base_asset = {"BTC"},
          .quote_asset = {"USDT"}}});

    const auto symbol_price_records =
        co_await symbols_db.SelectSymbolPriceRecords(eth_usdt, nullptr, nullptr,
                                                     nullptr);
    EXPECT_TRUE(symbol_price_records.empty());

    const auto eth_price_records = std::vector<stonks::core::SymbolPriceRecord>{
        stonks::core::SymbolPriceRecord{.symbol = eth_usdt,
                                        .buy_price = {0.1},
                                        .sell_price = {0.1},
                                        .time = absl::FromUnixMillis(1000)},
        stonks::core::SymbolPriceRecord{.symbol = eth_usdt,
                                        .buy_price = {0.2},
                                        .sell_price = {0.2},
                                        .time = absl::FromUnixMillis(2000)},
        stonks::core::SymbolPriceRecord{.symbol = eth_usdt,
                                        .buy_price = {0.3},
                                        .sell_price = {0.3},
                                        .time = absl::FromUnixMillis(3000)}};

    for (const auto &symbol_price_record : eth_price_records) {
      co_await symbols_db.InsertSymbolPriceRecord(symbol_price_record);
    }

    const auto btc_price_records = std::vector<stonks::core::SymbolPriceRecord>{
        stonks::core::SymbolPriceRecord{.symbol = btc_usdt,
                                        .buy_price = {0.1},
                                        .sell_price = {0.1},
                                        .time = absl::FromUnixMillis(10000)},
        stonks::core::SymbolPriceRecord{.symbol = btc_usdt,
                                        .buy_price = {0.2},
                                        .sell_price = {0.2},
                                        .time = absl::FromUnixMillis(20000)},
        stonks::core::SymbolPriceRecord{.symbol = btc_usdt,
                                        .buy_price = {0.3},
                                        .sell_price = {0.3},
                                        .time = absl::FromUnixMillis(30000)}};

    for (const auto &symbol_price_record : btc_price_records) {
      co_await symbols_db.InsertSymbolPriceRecord(symbol_price_record);
    }

    const auto eth_price_records_received =
        co_await symbols_db.SelectSymbolPriceRecords(eth_usdt, nullptr, nullptr,
                                                     nullptr);
    EXPECT_EQ(eth_price_records_received, eth_price_records);

    const auto btc_price_records_received =
        co_await symbols_db.SelectSymbolPriceRecords(btc_usdt, nullptr, nullptr,
                                                     nullptr);
    EXPECT_EQ(btc_price_records_received, btc_price_records);
  }());
}

TEST(AppSymbolsDb, SelectPeriod) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    const auto symbol = stonks::core::Symbol{"ETHUSDT"};
    auto price_records = co_await symbols_db.SelectSymbolPriceRecords(
        symbol, nullptr, nullptr, nullptr);
    EXPECT_EQ(price_records.size(), 3);

    price_records = co_await symbols_db.SelectSymbolPriceRecords(
        symbol, &static_cast<const absl::Time &>(absl::FromUnixMillis(1001)),
        nullptr, nullptr);
    EXPECT_EQ(price_records.size(), 2);

    price_records = co_await symbols_db.SelectSymbolPriceRecords(
        symbol, nullptr,
        &static_cast<const absl::Time &>(absl::FromUnixMillis(2999)), nullptr);
    EXPECT_EQ(price_records.size(), 2);

    price_records = co_await symbols_db.SelectSymbolPriceRecords(
        symbol, &static_cast<const absl::Time &>(absl::FromUnixMillis(1001)),
        &static_cast<const absl::Time &>(absl::FromUnixMillis(2999)), nullptr);
    EXPECT_EQ(price_records.size(), 1);
  }());
}

TEST(AppSymbolsDb, SelectLimit) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    const auto price_records = co_await symbols_db.SelectSymbolPriceRecords(
        {"ETHUSDT"}, nullptr, nullptr, &static_cast<const int &>(2));
    EXPECT_EQ(price_records.size(), 2);
  }());
}

TEST(AppSymbolsDb, SelectSymbolsWithPriceRecords) {
  cppcoro::sync_wait([]() -> cppcoro::task<> {
    const auto symbols = co_await symbols_db.SelectSymbolsWithPriceRecords();
    EXPECT_EQ(symbols.size(), 2);
  }());
}
}  // namespace