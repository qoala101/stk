#include "finance_db.h"

#include <gtest/gtest.h>

namespace {
const auto kTestDbFileName = "finance_db_test.db";

auto finance_db = std::optional<stonks::finance::FinanceDb>{};

TEST(FinanceDb, InsertAndSelect) {
  std::remove(kTestDbFileName);
  finance_db.emplace(kTestDbFileName);

  const auto assets = finance_db->SelectAssets();
  ASSERT_TRUE(assets.has_value());
  EXPECT_FALSE(assets->empty());

  const auto symbols = finance_db->SelectSymbols();
  ASSERT_TRUE(symbols.has_value());
  EXPECT_FALSE(symbols->empty());
}
}