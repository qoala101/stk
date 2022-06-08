#include "sqlite_db.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include "db_types.h"
#include "db_value.h"
#include "utils.h"

namespace {
const auto kTestDbFileName = "sqlite_db_test.db";

auto sqlite_db = std::optional<stonks::db::sqlite::SqliteDb>{};
auto &db = static_cast<stonks::db::Db &>(*sqlite_db);

TEST(SqliteDb, CreateAndDropTable) {
  static_cast<void>(std::remove(kTestDbFileName));
  sqlite_db.emplace(kTestDbFileName);

  const auto table = stonks::db::Table{"TestTable"};
  const auto table_definition = stonks::db::TableDefinition{
      .table = table,
      .columns = {
          stonks::db::ColumnDefinition{.column = "IntTrue",
                                       .data_type = stonks::db::DataType::kInt,
                                       .primary_key = true,
                                       .auto_increment = true,
                                       .unique = true},
          stonks::db::ColumnDefinition{
              .column = "TextFalse",
              .data_type = stonks::db::DataType::kString}}};
  db.CreateTableIfNotExists(table_definition);
  db.CreateTableIfNotExists(table_definition);
  db.DropTable(table);
  EXPECT_ANY_THROW(db.DropTable(table));
}

const auto kAssetTableDefinition = stonks::db::TableDefinition{
    .table = stonks::db::Table{"Asset"},
    .columns = {
        stonks::db::ColumnDefinition{.column = "id",
                                     .data_type = stonks::db::DataType::kInt64,
                                     .primary_key = true,
                                     .auto_increment = true,
                                     .unique = true},
        stonks::db::ColumnDefinition{.column = "name",
                                     .data_type = stonks::db::DataType::kString,
                                     .unique = true},
    }};

TEST(SqliteDb, InsertAndSelect) {
  db.CreateTableIfNotExists(kAssetTableDefinition);

  auto insert_statement =
      db.PrepareStatement("INSERT INTO \"" + kAssetTableDefinition.table +
                          "\"(\"name\") VALUES(?1)");
  insert_statement->Execute({std::string_view{"BTC"}});
  insert_statement->Execute({std::string_view{"ETH"}});
  insert_statement->Execute({std::string_view{"USDT"}});

  auto select_statement = db.PrepareStatement(
      "SELECT * FROM \"" + kAssetTableDefinition.table + "\"");

  const auto rows = select_statement->Execute({}, kAssetTableDefinition);
  EXPECT_EQ(rows.GetSize(), 3);
  EXPECT_EQ(rows.GetValues("id")[0].GetInt64(), 1);
  EXPECT_EQ(rows.GetValues("name")[0].GetString(), "BTC");
  EXPECT_EQ(rows.GetValues("id")[1].GetInt64(), 2);
  EXPECT_EQ(rows.GetValues("name")[1].GetString(), "ETH");
  EXPECT_EQ(rows.GetValues("id")[2].GetInt64(), 3);
  EXPECT_EQ(rows.GetValues("name")[2].GetString(), "USDT");
}

TEST(SqliteDb, InsertNull) {
  auto insert_statement =
      db.PrepareStatement("INSERT INTO \"" + kAssetTableDefinition.table +
                          "\"(\"name\") VALUES(?1)");
  EXPECT_ANY_THROW(insert_statement->Execute({}));
}

const auto kSymbolTableDefinition = stonks::db::TableDefinition{
    .table = "Symbol",
    .columns = {
        stonks::db::ColumnDefinition{.column = "id",
                                     .data_type = stonks::db::DataType::kInt64,
                                     .primary_key = true,
                                     .auto_increment = true,
                                     .unique = true},
        stonks::db::ColumnDefinition{
            .column = "base_asset_id",
            .data_type = stonks::db::DataType::kInt64,
            .foreign_key =
                stonks::db::ForeignKey{.table = "Asset", .column = "id"}},
        stonks::db::ColumnDefinition{.column = "quote_asset_id",
                                     .data_type = stonks::db::DataType::kInt64,
                                     .foreign_key = stonks::db::ForeignKey{
                                         .table = "Asset", .column = "id"}}}};

const auto kSymbolPriceTableDefinition = stonks::db::TableDefinition{
    .table = "SymbolPrice",
    .columns = {
        stonks::db::ColumnDefinition{
            .column = "symbol_id",
            .data_type = stonks::db::DataType::kInt64,
            .foreign_key =
                stonks::db::ForeignKey{.table = "Symbol", .column = "id"}},
        stonks::db::ColumnDefinition{.column = "time",
                                     .data_type = stonks::db::DataType::kInt64},
        stonks::db::ColumnDefinition{
            .column = "price", .data_type = stonks::db::DataType::kDouble}}};

TEST(SqliteDb, ForeignKey) {
  db.CreateTableIfNotExists(kSymbolTableDefinition);

  auto insert_symbol_statement = db.PrepareStatement(
      "INSERT INTO \"" + kSymbolTableDefinition.table +
      "\"(\"base_asset_id\", \"quote_asset_id\") VALUES(?1, ?2)");
  insert_symbol_statement->Execute({1, 3});
  insert_symbol_statement->Execute({2, 3});
  EXPECT_ANY_THROW(insert_symbol_statement->Execute({5, 6}));

  db.CreateTableIfNotExists(kSymbolPriceTableDefinition);

  auto insert_symbol_price_statement = db.PrepareStatement(
      "INSERT INTO \"" + kSymbolPriceTableDefinition.table +
      "\"(\"symbol_id\", \"time\", \"price\") VALUES(?1, ?2, ?3)");
  insert_symbol_price_statement->Execute(
      {1, stonks::utils::GetUnixTime().count(), 12345});
  insert_symbol_price_statement->Execute(
      {2, stonks::utils::GetUnixTime().count(), 0.12345});
}

TEST(SqliteDb, SelectJoin) {
  const auto cell_definitions = std::vector<stonks::db::CellDefinition>{
      stonks::db::CellDefinition{.column = "base_asset",
                                 .data_type = stonks::db::DataType::kString},
      stonks::db::CellDefinition{.column = "quote_asset",
                                 .data_type = stonks::db::DataType::kString}};

  auto select_statement = db.PrepareStatement(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id=BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id=QuoteAsset.id;");
  const auto rows = select_statement->Execute({}, cell_definitions);
  EXPECT_EQ(rows.GetSize(), 2);
  EXPECT_EQ(rows.GetValues("base_asset")[0].GetString(), "BTC");
  EXPECT_EQ(rows.GetValues("quote_asset")[0].GetString(), "USDT");
  EXPECT_EQ(rows.GetValues("base_asset")[1].GetString(), "ETH");
  EXPECT_EQ(rows.GetValues("quote_asset")[1].GetString(), "USDT");
}

TEST(SqliteDb, CascadeForeignKeyDelete) {
  db.DeleteFrom("Asset", "WHERE Asset.name = \"USDT\"");

  auto select_statement = db.PrepareStatement(
      "SELECT * FROM \"" + kAssetTableDefinition.table + "\"");
  auto rows = select_statement->Execute({}, kAssetTableDefinition);
  EXPECT_EQ(rows.GetSize(), 2);

  select_statement = db.PrepareStatement("SELECT * FROM \"" +
                                         kSymbolTableDefinition.table + "\"");
  rows = select_statement->Execute({}, kSymbolTableDefinition);
  EXPECT_EQ(rows.GetSize(), 0);

  select_statement = db.PrepareStatement(
      "SELECT * FROM \"" + kSymbolPriceTableDefinition.table + "\"");
  rows = select_statement->Execute({}, kSymbolPriceTableDefinition);
  EXPECT_EQ(rows.GetSize(), 0);
}
}  // namespace