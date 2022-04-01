#include "finance_api.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(FinanceApi, GetCandlesticks) {
  const auto candlesticks = stonks::GetCandlesticks("BTCUSDT");

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_FALSE(candlesticks->empty());
}