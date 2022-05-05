#include "sqlite_db.h"

#include <gtest/gtest.h>

#include <chrono>

#include "utils.h"

namespace {
const auto kTestDbFileName = "sqlite_db_test.db";

auto sqlite_db = std::optional<stonks::db::sqlite::SqliteDb>{};
auto &db = static_cast<stonks::db::Db &>(*sqlite_db);

TEST(SqliteDb, CreateAndDropTable) {
  std::remove(kTestDbFileName);
  sqlite_db =
      stonks::db::sqlite::SqliteDb::OpenOrCreateDbFromFile(kTestDbFileName);

  const auto table = stonks::db::Table{.name = "TestTable"};
  const auto table_definition = stonks::db::TableDefinition{
      .table = table,
      .columns = {
          stonks::db::Column{.name = "IntTrue",
                             .data_type = stonks::db::DataType::kInteger,
                             .primary_key = true,
                             .auto_increment = true,
                             .unique = true},
          stonks::db::Column{.name = "TextFalse",
                             .data_type = stonks::db::DataType::kText}}};
  auto success = db.CreateTable(table_definition);
  EXPECT_TRUE(success);

  success = db.CreateTable(table_definition);
  EXPECT_FALSE(success);

  success = db.DropTable(table);
  EXPECT_TRUE(success);

  success = db.DropTable(table);
  EXPECT_FALSE(success);
}

TEST(SqliteDb, InsertAndSelect) {
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
  auto success = db.CreateTable(table_definition);
  EXPECT_TRUE(success);
  success = db.Insert(
      table, stonks::db::Row{.cells = {stonks::db::Cell{.column_name = "name",
                                                        .value = "BTC"}}});
  EXPECT_TRUE(success);
  success = db.Insert(
      table, stonks::db::Row{.cells = {stonks::db::Cell{.column_name = "name",
                                                        .value = "ETH"}}});
  EXPECT_TRUE(success);
  success = db.Insert(
      table, stonks::db::Row{.cells = {stonks::db::Cell{.column_name = "name",
                                                        .value = "USDT"}}});
  EXPECT_TRUE(success);

  const auto rows = db.Select(table_definition);
  ASSERT_TRUE(rows.has_value());
  EXPECT_EQ(rows->size(), 3);

  EXPECT_EQ((*rows)[0].cells.size(), 2);
  EXPECT_EQ((*rows)[0].cells[0].column_name, "id");
  ASSERT_TRUE((*rows)[0].cells[0].value.GetInt64().has_value());
  EXPECT_EQ(*(*rows)[0].cells[0].value.GetInt64(), 1);
  EXPECT_EQ((*rows)[0].cells[1].column_name, "name");
  ASSERT_TRUE((*rows)[0].cells[1].value.GetString() != nullptr);
  EXPECT_EQ(*(*rows)[0].cells[1].value.GetString(), "BTC");

  EXPECT_EQ((*rows)[1].cells.size(), 2);
  EXPECT_EQ((*rows)[1].cells[0].column_name, "id");
  ASSERT_TRUE((*rows)[1].cells[0].value.GetInt64().has_value());
  EXPECT_EQ(*(*rows)[1].cells[0].value.GetInt64(), 2);
  EXPECT_EQ((*rows)[1].cells[1].column_name, "name");
  ASSERT_TRUE((*rows)[1].cells[1].value.GetString() != nullptr);
  EXPECT_EQ(*(*rows)[1].cells[1].value.GetString(), "ETH");

  EXPECT_EQ((*rows)[2].cells.size(), 2);
  EXPECT_EQ((*rows)[2].cells[0].column_name, "id");
  ASSERT_TRUE((*rows)[2].cells[0].value.GetInt64().has_value());
  EXPECT_EQ(*(*rows)[2].cells[0].value.GetInt64(), 3);
  EXPECT_EQ((*rows)[2].cells[1].column_name, "name");
  ASSERT_TRUE((*rows)[2].cells[1].value.GetString() != nullptr);
  EXPECT_EQ(*(*rows)[2].cells[1].value.GetString(), "USDT");
}

TEST(SqliteDb, InsertNull) {
  const auto success =
      db.Insert(stonks::db::Table{.name = "Asset"}, stonks::db::Row{});
  EXPECT_FALSE(success);
}

TEST(SqliteDb, ForeignKey) {
  auto table = stonks::db::Table{.name = "Symbol"};
  auto table_definition = stonks::db::TableDefinition{
      .table = table,
      .columns = {
          stonks::db::Column{.name = "id",
                             .data_type = stonks::db::DataType::kInteger,
                             .primary_key = true,
                             .auto_increment = true,
                             .unique = true},
          stonks::db::Column{
              .name = "base_asset_id",
              .data_type = stonks::db::DataType::kInteger,
              .foreign_key = stonks::db::ForeignKey{.table_name = "Asset",
                                                    .column_name = "id"}},
          stonks::db::Column{.name = "quote_asset_id",
                             .data_type = stonks::db::DataType::kInteger,
                             .foreign_key = stonks::db::ForeignKey{
                                 .table_name = "Asset", .column_name = "id"}}}};
  auto success = db.CreateTable(table_definition);
  EXPECT_TRUE(success);
  success = db.Insert(
      table,
      stonks::db::Row{
          .cells = {
              stonks::db::Cell{.column_name = "base_asset_id", .value = 1},
              stonks::db::Cell{.column_name = "quote_asset_id", .value = 3}}});
  EXPECT_TRUE(success);
  success = db.Insert(
      table,
      stonks::db::Row{
          .cells = {
              stonks::db::Cell{.column_name = "base_asset_id", .value = 2},
              stonks::db::Cell{.column_name = "quote_asset_id", .value = 3}}});
  EXPECT_TRUE(success);
  success = db.Insert(
      table,
      stonks::db::Row{
          .cells = {
              stonks::db::Cell{.column_name = "base_asset_id", .value = 5},
              stonks::db::Cell{.column_name = "quote_asset_id", .value = 6}}});
  EXPECT_FALSE(success);

  table = stonks::db::Table{.name = "SymbolPrice"};
  table_definition = stonks::db::TableDefinition{
      .table = table,
      .columns = {
          stonks::db::Column{
              .name = "symbol_id",
              .data_type = stonks::db::DataType::kInteger,
              .foreign_key = stonks::db::ForeignKey{.table_name = "Symbol",
                                                    .column_name = "id"}},
          stonks::db::Column{.name = "time",
                             .data_type = stonks::db::DataType::kInteger},
          stonks::db::Column{.name = "price",
                             .data_type = stonks::db::DataType::kReal}}};
  success = db.CreateTable(table_definition);
  EXPECT_TRUE(success);
  success = db.Insert(
      table,
      stonks::db::Row{
          .cells = {
              stonks::db::Cell{.column_name = "symbol_id", .value = 1},
              stonks::db::Cell{.column_name = "time",
                               .value = stonks::utils::GetUnixTime().count()},
              stonks::db::Cell{.column_name = "price",
                               .value = double{12345}}}});
  EXPECT_TRUE(success);
  success = db.Insert(
      table,
      stonks::db::Row{
          .cells = {
              stonks::db::Cell{.column_name = "symbol_id", .value = 2},
              stonks::db::Cell{.column_name = "time",
                               .value = stonks::utils::GetUnixTime().count()},
              stonks::db::Cell{.column_name = "price", .value = 0.12345}}});
  EXPECT_TRUE(success);
}

TEST(SqliteDb, SelectJoin) {
  const auto columns = std::vector<stonks::db::Column>{
      stonks::db::Column{.name = "base_asset",
                         .data_type = stonks::db::DataType::kText},
      stonks::db::Column{.name = "quote_asset",
                         .data_type = stonks::db::DataType::kText}};
  const auto rows = db.Select(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id=BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id=QuoteAsset.id;",
      columns);
  ASSERT_TRUE(rows.has_value());
  EXPECT_EQ(rows->size(), 2);

  EXPECT_EQ((*rows)[0].cells.size(), 2);
  ASSERT_NE((*rows)[0].GetCellValueString("base_asset"), nullptr);
  EXPECT_EQ(*(*rows)[0].GetCellValueString("base_asset"), "BTC");
  ASSERT_NE((*rows)[0].GetCellValueString("quote_asset"), nullptr);
  EXPECT_EQ(*(*rows)[0].GetCellValueString("quote_asset"), "USDT");

  EXPECT_EQ((*rows)[1].cells.size(), 2);
  ASSERT_NE((*rows)[1].GetCellValueString("base_asset"), nullptr);
  EXPECT_EQ(*(*rows)[1].GetCellValueString("base_asset"), "ETH");
  ASSERT_NE((*rows)[1].GetCellValueString("quote_asset"), nullptr);
  EXPECT_EQ(*(*rows)[1].GetCellValueString("quote_asset"), "USDT");
}
}  // namespace