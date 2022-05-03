#include "sqlite_db.h"

#include <gtest/gtest.h>

#include <chrono>

namespace {
auto sqlite_db = stonks::db::SqliteDb{};
auto &db = static_cast<stonks::db::Db &>(sqlite_db);
}  // namespace

TEST(Db, CreateSameTableError) {
  const auto table_definition = stonks::db::TableDefinition{
      .table = stonks::db::Table{.name = "TestTable"}};
  auto error = db.CreateTable(table_definition);
  EXPECT_FALSE(error.has_value());

  error = db.CreateTable(table_definition);
  ASSERT_TRUE(error.has_value());
  EXPECT_FALSE(error->message.empty());
}

TEST(Db, DropTable) {
  const auto table = stonks::db::Table{.name = "TestTable"};

  auto table_definition = db.GetTableDefinition(table).GetValue();
  EXPECT_TRUE(table_definition.has_value());

  const auto error = db.DropTable(table);
  EXPECT_FALSE(error.has_value());

  const auto table_definition_result = db.GetTableDefinition(table);
  EXPECT_FALSE(table_definition_result.GetValue().has_value());
  EXPECT_TRUE(table_definition_result.GetError().has_value());
}

TEST(Db, InsertAndSelect) {
  const auto table = stonks::db::Table{.name = "Asset"};
  const auto table_definition = stonks::db::TableDefinition{
      .table = table,
      .columns = {
          stonks::db::Column{.name = "id",
                             .data_type = stonks::db::DataType::kInteger,
                             .primary_key = true,
                             .auto_increment = true,
                             .unique = true},
          stonks::db::Column{.name = "name",
                             .data_type = stonks::db::DataType::kText,
                             .unique = true},
      }};
  const auto error = db.CreateTable(table_definition);
  ASSERT_FALSE(error.has_value());
  const auto stored_table_definition_ = db.GetTableDefinition(table).GetValue();
  ASSERT_TRUE(stored_table_definition_.has_value());
  EXPECT_EQ(*stored_table_definition_, table_definition);

  db.Insert(
      stonks::db::Table{.name = "Asset"},
      stonks::db::Row{
          .cells = {stonks::db::Cell{.column_name = "name", .value = "BTC"},
                    stonks::db::Cell{.column_name = "name", .value = "ETH"},
                    stonks::db::Cell{.column_name = "name", .value = "USDT"}}});

  const auto rows = db.Select(stonks::db::Table{.name = "Asset"});
  ASSERT_TRUE(rows.has_value());
  EXPECT_EQ(rows->size(), 3);

  EXPECT_EQ((*rows)[0].cells.size(), 2);
  EXPECT_EQ((*rows)[0].cells[0].column_name, "id");
  ASSERT_TRUE((*rows)[0].cells[0].value.GetInt64().has_value());
  EXPECT_EQ(*(*rows)[0].cells[0].value.GetInt64(), 1);
  EXPECT_EQ((*rows)[0].cells[1].column_name, "name");
  ASSERT_TRUE((*rows)[0].cells[1].value.GetString().has_value());
  EXPECT_EQ(*(*rows)[0].cells[1].value.GetString(), "BTC");

  EXPECT_EQ((*rows)[1].cells.size(), 2);
  EXPECT_EQ((*rows)[1].cells[0].column_name, "id");
  ASSERT_TRUE((*rows)[1].cells[0].value.GetInt64().has_value());
  EXPECT_EQ(*(*rows)[1].cells[0].value.GetInt64(), 2);
  EXPECT_EQ((*rows)[1].cells[1].column_name, "name");
  ASSERT_TRUE((*rows)[1].cells[1].value.GetString().has_value());
  EXPECT_EQ(*(*rows)[1].cells[1].value.GetString(), "ETH");

  EXPECT_EQ((*rows)[2].cells.size(), 2);
  EXPECT_EQ((*rows)[2].cells[0].column_name, "id");
  ASSERT_TRUE((*rows)[2].cells[0].value.GetInt64().has_value());
  EXPECT_EQ(*(*rows)[2].cells[0].value.GetInt64(), 3);
  EXPECT_EQ((*rows)[2].cells[1].column_name, "name");
  ASSERT_TRUE((*rows)[2].cells[1].value.GetString().has_value());
  EXPECT_EQ(*(*rows)[2].cells[1].value.GetString(), "USDT");
}

// TEST(Db, ForeignKey) {
//   auto table = stonks::db::Table{.name = "Symbol"};
//   auto table_definition = stonks::db::TableDefinition{
//       .table = table,
//       .columns = {
//           stonks::db::Column{.name = "id",
//                              .data_type = stonks::db::DataType::kInteger,
//                              .primary_key = true,
//                              .auto_increment = true,
//                              .unique = true},
//           stonks::db::Column{
//               .name = "base_asset_id",
//               .data_type = stonks::db::DataType::kInteger,
//               .foreign_key = stonks::db::ForeignKey{.table_name = "Asset",
//                                                     .column_name = "id"}},
//           stonks::db::Column{.name = "quote_asset_id",
//                              .data_type = stonks::db::DataType::kInteger,
//                              .foreign_key = stonks::db::ForeignKey{
//                                  .table_name = "Asset", .column_name =
//                                  "id"}}}};
//   auto error = db.CreateTable(table_definition);
//   ASSERT_FALSE(error.has_value());
//   auto stored_table_definition_ = db.GetTableDefinition(table).GetValue();
//   ASSERT_TRUE(stored_table_definition_.has_value());
//   EXPECT_EQ(*stored_table_definition_, table_definition);

//   table = stonks::db::Table{.name = "SymbolPrice"};
//   table_definition = stonks::db::TableDefinition{
//       .table = table,
//       .columns = {
//           stonks::db::Column{
//               .name = "symbol_id",
//               .data_type = stonks::db::DataType::kInteger,
//               .foreign_key = stonks::db::ForeignKey{.table_name = "Symbol",
//                                                     .column_name = "id"}},
//           stonks::db::Column{.name = "time",
//                              .data_type = stonks::db::DataType::kInteger},
//           stonks::db::Column{.name = "price",
//                              .data_type = stonks::db::DataType::kReal}}};
//   error = db.CreateTable(table_definition);
//   ASSERT_FALSE(error.has_value());
//   stored_table_definition_ = db.GetTableDefinition(table).GetValue();
//   ASSERT_TRUE(stored_table_definition_.has_value());
//   EXPECT_EQ(*stored_table_definition_, table_definition);
// }