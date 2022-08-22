#include <gtest/gtest.h>

#include <string>

int main(int, const char *[]) {
  testing::InitGoogleTest();
  // testing::GTEST_FLAG(filter) = "RestRequestSender*";
  // testing::GTEST_FLAG(filter) = "RestRequestReceiver*";
  // testing::GTEST_FLAG(filter) = "BinanceApi*";
  // testing::GTEST_FLAG(filter) = "PricesStream*";
  // testing::GTEST_FLAG(filter) = "JsonConversions*";
  // testing::GTEST_FLAG(filter) = "WebSocket*";
  // testing::GTEST_FLAG(filter) = "FinanceEnum*";
  // testing::GTEST_FLAG(filter) = "FinanceApi*";
  // testing::GTEST_FLAG(filter) = "Utils*";
  // testing::GTEST_FLAG(filter) = "CandlesStream*";
  // testing::GTEST_FLAG(filter) = "BufferedCandlesStream*";
  // testing::GTEST_FLAG(filter) = "SqliteDb*";
  // testing::GTEST_FLAG(filter) = "StonksDb*";
  // testing::GTEST_FLAG(filter) = "WebSocket*";
  // testing::GTEST_FLAG(filter) = "StrategyRunner*";
  // testing::GTEST_FLAG(filter) = "PriceTicksDbStream*";
  // testing::GTEST_FLAG(filter) = "Service*";
  testing::GTEST_FLAG(filter) = "ClientServer*";
  // testing::GTEST_FLAG(filter) = "FinanceDbClientServer*";
  return RUN_ALL_TESTS();
}