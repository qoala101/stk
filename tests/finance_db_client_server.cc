#include <gtest/gtest.h>

#include <limits>

#include "finance_db.h"
#include "finance_db_client.h"
#include "finance_db_server.h"
#include "finance_types.h"

namespace {
const auto kBaseUri = "http://localhost:6506/Entity";

template <stonks::finance::FinanceDbConcept T>
void SetupDb(T &t) {
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

  for (const auto &price_tick : eth_price_ticks) {
    EXPECT_TRUE(t.InsertSymbolPriceTick(price_tick));
  }
}

template <stonks::finance::FinanceDbConcept L,
          stonks::finance::FinanceDbConcept R>
void Compare(const L &l, const R &r) {
  EXPECT_EQ(l.SelectAssets(), r.SelectAssets());
  EXPECT_EQ(l.SelectSymbols(), r.SelectSymbols());
  EXPECT_EQ(l.SelectSymbolPriceTicks(), r.SelectSymbolPriceTicks());
  EXPECT_EQ(l.SelectSymbolPriceTicks(10), r.SelectSymbolPriceTicks(10));
  EXPECT_EQ(l.SelectSymbolPriceTicks(
                10, stonks::finance::Period{.start_time =
                                                std::chrono::milliseconds{0}}),
            r.SelectSymbolPriceTicks(
                10, stonks::finance::Period{.start_time =
                                                std::chrono::milliseconds{0}}));
  EXPECT_EQ(
      l.SelectSymbolPriceTicks(
          10,
          stonks::finance::Period{.end_time = std::chrono::milliseconds{2999}}),
      r.SelectSymbolPriceTicks(
          10, stonks::finance::Period{.end_time =
                                          std::chrono::milliseconds{2999}}));
  EXPECT_EQ(
      l.SelectSymbolPriceTicks(10, std::nullopt,
                               std::vector<stonks::finance::Symbol>{
                                   stonks::finance::Symbol{"ETH", "USDT"}}),
      r.SelectSymbolPriceTicks(10, std::nullopt,
                               std::vector<stonks::finance::Symbol>{
                                   stonks::finance::Symbol{"ETH", "USDT"}}));
  EXPECT_EQ(l.SelectSymbolPriceTicks(
                10, std::nullopt,
                std::vector<stonks::finance::Symbol>{
                    stonks::finance::Symbol{"BAD_ASSET", "USDT"}}),
            r.SelectSymbolPriceTicks(
                10, std::nullopt,
                std::vector<stonks::finance::Symbol>{
                    stonks::finance::Symbol{"BAD_ASSET", "USDT"}}));
}

TEST(FinanceDbClientServer, Requests) {
  auto finance_db = stonks::finance::FinanceDb{":memory:"};

  const auto finance_db_server =
      stonks::finance::FinanceDbServer{kBaseUri, ":memory:"};
  auto finance_db_client = stonks::finance::FinanceDbClient{kBaseUri};

  SetupDb(finance_db);
  SetupDb(finance_db_client);
  Compare(finance_db, finance_db_client);
}
}  // namespace