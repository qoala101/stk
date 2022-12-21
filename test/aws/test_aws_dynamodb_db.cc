#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <cppcoro/sync_wait.hpp>
#include <memory>
#include <not_null.hpp>
#include <optional>
#include <string>
#include <tuple>

#include "gtest/gtest_pred_impl.h"
#include "kvdb_i_items_interface.h"
#include "kvdb_i_tables_interface.h"
#include "kvdb_types.h"
#include "test_aws_injector.h"

namespace {
TEST(DynamoDb, Test1) {
  auto tables_interface =
      test::aws::Injector()
          .create<stonks::cpp::NnUp<stonks::kvdb::ITablesInterface>>();
  auto items_interface =
      test::aws::Injector()
          .create<stonks::cpp::NnUp<stonks::kvdb::IItemsInterface>>();

  cppcoro::sync_wait([&tables_interface,
                      &items_interface]() -> cppcoro::task<> {
    static const auto kTable = stonks::kvdb::Table{"TestTable"};

    co_await tables_interface->DropTableIfExists(kTable);
    co_await tables_interface->CreateTableIfNotExists(kTable);

    const auto item1 =
        stonks::kvdb::Item{.key = {"TestKey1"}, .value = "TestValue1"};
    EXPECT_EQ(co_await items_interface->SelectItem(kTable, item1.key),
              std::nullopt);

    co_await items_interface->InsertOrUpdateItem(kTable, item1);
    EXPECT_EQ(co_await items_interface->SelectItem(kTable, item1.key), item1);

    const auto item1_new_value =
        stonks::kvdb::Item{.key = item1.key, .value = "TestValue2"};
    co_await items_interface->InsertOrUpdateItem(kTable, item1_new_value);
    EXPECT_EQ(co_await items_interface->SelectItem(kTable, item1.key),
              item1_new_value);

    const auto item2 =
        stonks::kvdb::Item{.key = {"TestKey2"}, .value = "TestValue2"};
    co_await items_interface->InsertOrUpdateItem(kTable, item2);
    EXPECT_EQ(co_await items_interface->SelectItem(kTable, item2.key), item2);

    co_await items_interface->DeleteItemIfExists(kTable, item1.key);
    EXPECT_EQ(co_await items_interface->SelectItem(kTable, item1.key),
              std::nullopt);

    co_await tables_interface->DropTableIfExists(kTable);
    EXPECT_ANY_THROW(
        std::ignore = co_await items_interface->SelectItem(kTable, item1.key));
    EXPECT_ANY_THROW(
        co_await items_interface->DeleteItemIfExists(kTable, item2.key));
  }());
}
}  // namespace