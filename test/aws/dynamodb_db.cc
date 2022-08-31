#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <memory>
#include <optional>
#include <string>
#include <tuple>

#include "aws_dynamodb_factory.h"
#include "gtest/gtest_pred_impl.h"
#include "nosqldb_types.h"
#include "not_null.hpp"

namespace {
TEST(DynamoDb, Test1) {
  auto db = stonks::aws::dynamodb::Factory{}.CreateDb();

  static const auto kTable = stonks::nosqldb::Table{"TestTable"};

  db->DropTableIfExists(kTable);
  db->CreateTableIfNotExists(kTable);

  const auto item1 =
      stonks::nosqldb::Item{.key = "TestKey1", .value = "TestValue1"};
  ASSERT_EQ(db->SelectItem(kTable, item1.key), std::nullopt);

  db->InsertOrUpdateItem(kTable, item1);
  ASSERT_EQ(db->SelectItem(kTable, item1.key), item1);

  const auto item1_new_value =
      stonks::nosqldb::Item{.key = item1.key, .value = "TestValue2"};
  db->InsertOrUpdateItem(kTable, item1_new_value);
  ASSERT_EQ(db->SelectItem(kTable, item1.key), item1_new_value);

  const auto item2 =
      stonks::nosqldb::Item{.key = "TestKey2", .value = "TestValue2"};
  db->InsertOrUpdateItem(kTable, item2);
  ASSERT_EQ(db->SelectItem(kTable, item2.key), item2);

  db->DeleteItemIfExists(kTable, item1.key);
  ASSERT_EQ(db->SelectItem(kTable, item1.key), std::nullopt);

  db->DropTableIfExists(kTable);
  EXPECT_ANY_THROW(std::ignore = db->SelectItem(kTable, item1.key));
  EXPECT_ANY_THROW(db->DeleteItemIfExists(kTable, item2.key));
}
}  // namespace