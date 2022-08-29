#include <gtest/gtest.h>
#include "aws_dynamodb_factory.h"

TEST(DynamoDb, Test1) {
  auto db = stonks::aws::Factory{}.LoadDbFromUri("");
  db->CreateTableIfNotExists("Test");
  const auto value = db->SelectItem("Test", "TestKey");
  std::cout << value;
}