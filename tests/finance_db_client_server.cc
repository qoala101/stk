#include "finance_db_client_server.h"

#include <gtest/gtest.h>

#include <limits>
#include <memory>

#include "stonks_db_updater_symbols_info.h"
#include "finance_db.h"
#include "finance_types.h"
#include "stonks_db.h"

namespace {
const auto kBaseUri = "http://localhost:6506/Entity";

void SetupDb(const std::shared_ptr<stonks::StonksDb> &stonks_db) {
  static_cast<void>(stonks::DbUpdaterSymbolsInfo{stonks_db});
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

  for (const auto &price_tick : eth_price_ticks) {
    stonks_db->InsertSymbolPriceTick(price_tick);
  }
}

void Compare(const stonks::StonksDb &left, const stonks::StonksDb &right) {
  EXPECT_EQ(left.SelectAssets(), right.SelectAssets());
  EXPECT_EQ(left.SelectSymbols(), right.SelectSymbols());
  EXPECT_EQ(
      left.SelectSymbolPriceTicks(std::nullopt, std::nullopt, std::nullopt),
      right.SelectSymbolPriceTicks(std::nullopt, std::nullopt, std::nullopt));
  EXPECT_EQ(left.SelectSymbolPriceTicks(10, std::nullopt, std::nullopt),
            right.SelectSymbolPriceTicks(10, std::nullopt, std::nullopt));
  EXPECT_EQ(
      left.SelectSymbolPriceTicks(
          10,
          stonks::finance::Period{.start_time = std::chrono::milliseconds{0}},
          std::nullopt),
      right.SelectSymbolPriceTicks(
          10,
          stonks::finance::Period{.start_time = std::chrono::milliseconds{0}},
          std::nullopt));
  EXPECT_EQ(
      left.SelectSymbolPriceTicks(
          10,
          stonks::finance::Period{.end_time = std::chrono::milliseconds{2999}},
          std::nullopt),
      right.SelectSymbolPriceTicks(
          10,
          stonks::finance::Period{.end_time = std::chrono::milliseconds{2999}},
          std::nullopt));
  EXPECT_EQ(left.SelectSymbolPriceTicks(
                10, std::nullopt,
                std::vector<stonks::finance::SymbolName>{"ETHUSDT"}),
            right.SelectSymbolPriceTicks(
                10, std::nullopt,
                std::vector<stonks::finance::SymbolName>{"ETHUSDT"}));
  EXPECT_EQ(left.SelectSymbolPriceTicks(
                10, std::nullopt,
                std::vector<stonks::finance::SymbolName>{"BAD_ASSETUSDT"}),
            right.SelectSymbolPriceTicks(
                10, std::nullopt,
                std::vector<stonks::finance::SymbolName>{"BAD_ASSETUSDT"}));
}

TEST(FinanceDbClientServer, Requests) {
  auto local_finance_db =
      std::make_shared<stonks::finance::FinanceDb>(":memory:");
  auto server_finance_db =
      std::make_shared<stonks::finance::FinanceDb>(":memory:");

  const auto server_db =
      stonks::finance::StonksDbServer{kBaseUri, server_finance_db};
  auto finance_db_client =
      std::make_shared<stonks::finance::FinanceDbClient>(kBaseUri);

  SetupDb(local_finance_db);
  SetupDb(finance_db_client);
  Compare(*local_finance_db, *finance_db_client);
}
}  // namespace