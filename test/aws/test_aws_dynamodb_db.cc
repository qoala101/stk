#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <memory>
#include <optional>
#include <string>
#include <tuple>

#include "gtest/gtest_pred_impl.h"
#include "nosqldb_i_items_interface.h"
#include "nosqldb_i_tables_interface.h"
#include "nosqldb_types.h"
#include "not_null.hpp"
#include "test_aws_injector.h"

namespace {
TEST(DynamoDb, Test1) {
  auto tables_interface =
      test::aws::Injector()
          .create<stonks::cpp::NnUp<stonks::nosqldb::ITablesInterface>>();
  auto items_interface =
      test::aws::Injector()
          .create<stonks::cpp::NnUp<stonks::nosqldb::IItemsInterface>>();

  static const auto kTable = stonks::nosqldb::Table{"TestTable"};

  tables_interface->DropTableIfExists(kTable);
  tables_interface->CreateTableIfNotExists(kTable);

  const auto item1 =
      stonks::nosqldb::Item{.key = {"TestKey1"}, .value = "TestValue1"};
  ASSERT_EQ(items_interface->SelectItem(kTable, item1.key), std::nullopt);

  items_interface->InsertOrUpdateItem(kTable, item1);
  ASSERT_EQ(items_interface->SelectItem(kTable, item1.key), item1);

  const auto item1_new_value =
      stonks::nosqldb::Item{.key = item1.key, .value = "TestValue2"};
  items_interface->InsertOrUpdateItem(kTable, item1_new_value);
  ASSERT_EQ(items_interface->SelectItem(kTable, item1.key), item1_new_value);

  const auto item2 =
      stonks::nosqldb::Item{.key = {"TestKey2"}, .value = "TestValue2"};
  items_interface->InsertOrUpdateItem(kTable, item2);
  ASSERT_EQ(items_interface->SelectItem(kTable, item2.key), item2);

  items_interface->DeleteItemIfExists(kTable, item1.key);
  ASSERT_EQ(items_interface->SelectItem(kTable, item1.key), std::nullopt);

  tables_interface->DropTableIfExists(kTable);
  EXPECT_ANY_THROW(std::ignore =
                       items_interface->SelectItem(kTable, item1.key));
  EXPECT_ANY_THROW(items_interface->DeleteItemIfExists(kTable, item2.key));
}
}  // namespace