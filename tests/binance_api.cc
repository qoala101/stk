#include "binance_api.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(BinanceApi, SimpleTest)
{
  const auto symbols = stonks::binance::GetSymbols();
  EXPECT_TRUE(symbols.has_value());

  const auto balances = stonks::binance::GetBalances();
  EXPECT_TRUE(balances.has_value());
}