#include <gtest/gtest.h>

#include <memory>
#include <optional>

#include "aws_dynamodb_factory.h"

TEST(DynamoDb, Test1) {
  auto db = stonks::aws::dynamodb::Factory{}.LoadDbFromUri("");
  db->DropTable("TestTable");
  db->CreateTableIfNotExists("TestTable");

  ASSERT_EQ(db->SelectItem("TestTable", "TestKey1"), std::nullopt);

  db->InsertOrUpdateItem("TestTable", "TestKey1", "TestValue1");
  ASSERT_EQ(db->SelectItem("TestTable", "TestKey1"), "TestValue1");

  db->InsertOrUpdateItem("TestTable", "TestKey1", "TestValue2");
  ASSERT_EQ(db->SelectItem("TestTable", "TestKey1"), "TestValue2");

  db->InsertOrUpdateItem("TestTable", "TestKey2", "TestValue2");
  ASSERT_EQ(db->SelectItem("TestTable", "TestKey2"), "TestValue2");

  db->DeleteItem("TestTable", "TestKey1");
  ASSERT_EQ(db->SelectItem("TestTable", "TestKey1"), std::nullopt);

  db->DropTable("TestTable");
  EXPECT_ANY_THROW(std::ignore = db->SelectItem("TestTable", "TestKey2"));
}