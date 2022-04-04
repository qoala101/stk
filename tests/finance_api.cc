#include "finance_api.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "utils.h"

TEST(FinanceApi, GetCandlesticks) {
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour,
      *stonks::utils::GetUnixTimeMillisFromString("1 Apr 2021 00:00:00"),
      *stonks::utils::GetUnixTimeMillisFromString("1 Apr 2022 00:00:00"));

  ASSERT_TRUE(candlesticks.has_value());
  EXPECT_FALSE(candlesticks->empty());
}