#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include "sqldb_db.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqlite_db_factory.h"
#include "utils.h"

namespace {
const auto kTestDbFileName = "sqlite_db_test.db";

auto db_factory = std::unique_ptr<stonks::sqldb::IDbFactory>{};
auto db = std::unique_ptr<stonks::sqldb::IDb>{};

TEST(SqliteDb, CreateAndDropTable) {
  static_cast<void>(std::filesystem::remove(kTestDbFileName));
  db_factory = std::make_unique<stonks::sqlite::SqliteDbFactory>();
  db = db_factory->LoadDbFromFile(kTestDbFileName);

  const auto table = stonks::sqldb::Table{"TestTable"};
  const auto table_definition = stonks::sqldb::TableDefinition{
      .table = table,
      .columns = {stonks::sqldb::ColumnDefinition{
                      .column = "IntTrue",
                      .data_type = stonks::sqldb::DataType::kInt,
                      .primary_key = true,
                      .auto_increment = true,
                      .unique = true},
                  stonks::sqldb::ColumnDefinition{
                      .column = "TextFalse",
                      .data_type = stonks::sqldb::DataType::kString}}};
  db->PrepareStatement(
        db_factory->CreateQueryBuilder()->BuildCreateTableIfNotExistsQuery(
            table_definition))
      ->Execute();
  db->PrepareStatement(
        db_factory->CreateQueryBuilder()->BuildCreateTableIfNotExistsQuery(
            table_definition))
      ->Execute();
  db->PrepareStatement(
        db_factory->CreateQueryBuilder()->BuildDropTableQuery(table))
      ->Execute();
  EXPECT_ANY_THROW(
      db->PrepareStatement(
            db_factory->CreateQueryBuilder()->BuildDropTableQuery(table))
          ->Execute());
}

const auto kAssetTableDefinition = stonks::sqldb::TableDefinition{
    .table = stonks::sqldb::Table{"Asset"},
    .columns = {
        stonks::sqldb::ColumnDefinition{
            .column = "id",
            .data_type = stonks::sqldb::DataType::kInt64,
            .primary_key = true,
            .auto_increment = true,
            .unique = true},
        stonks::sqldb::ColumnDefinition{
            .column = "name",
            .data_type = stonks::sqldb::DataType::kString,
            .unique = true},
    }};

TEST(SqliteDb, InsertAndSelect) {
  db->PrepareStatement(
        db_factory->CreateQueryBuilder()->BuildCreateTableIfNotExistsQuery(
            kAssetTableDefinition))
      ->Execute();

  auto insert_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildInsertQuery(
          kAssetTableDefinition));
  insert_statement->Execute({{}, std::string_view{"BTC"}});
  insert_statement->Execute({{}, std::string_view{"ETH"}});
  insert_statement->Execute({{}, std::string_view{"USDT"}});

  auto select_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildSelectQuery(
                               kAssetTableDefinition.table),
                           kAssetTableDefinition);

  const auto rows = select_statement->Execute();
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
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildInsertQuery(
          kAssetTableDefinition));
  EXPECT_ANY_THROW(insert_statement->Execute());
}

const auto kSymbolTableDefinition = stonks::sqldb::TableDefinition{
    .table = "Symbol",
    .columns = {stonks::sqldb::ColumnDefinition{
                    .column = "id",
                    .data_type = stonks::sqldb::DataType::kInt64,
                    .primary_key = true,
                    .auto_increment = true,
                    .unique = true},
                stonks::sqldb::ColumnDefinition{
                    .column = "base_asset_id",
                    .data_type = stonks::sqldb::DataType::kInt64,
                    .foreign_key = stonks::sqldb::ForeignKey{.table = "Asset",
                                                             .column = "id"}},
                stonks::sqldb::ColumnDefinition{
                    .column = "quote_asset_id",
                    .data_type = stonks::sqldb::DataType::kInt64,
                    .foreign_key = stonks::sqldb::ForeignKey{.table = "Asset",
                                                             .column = "id"}}}};

const auto kSymbolPriceTableDefinition = stonks::sqldb::TableDefinition{
    .table = "SymbolPrice",
    .columns = {
        stonks::sqldb::ColumnDefinition{
            .column = "symbol_id",
            .data_type = stonks::sqldb::DataType::kInt64,
            .foreign_key =
                stonks::sqldb::ForeignKey{.table = "Symbol", .column = "id"}},
        stonks::sqldb::ColumnDefinition{
            .column = "time", .data_type = stonks::sqldb::DataType::kInt64},
        stonks::sqldb::ColumnDefinition{
            .column = "price", .data_type = stonks::sqldb::DataType::kDouble}}};

TEST(SqliteDb, ForeignKey) {
  db->PrepareStatement(
        db_factory->CreateQueryBuilder()->BuildCreateTableIfNotExistsQuery(
            kSymbolTableDefinition))
      ->Execute();

  auto insert_symbol_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildInsertQuery(
          kSymbolTableDefinition.table, {"base_asset_id", "quote_asset_id"}));
  insert_symbol_statement->Execute({1, 3});
  insert_symbol_statement->Execute({2, 3});
  EXPECT_ANY_THROW(insert_symbol_statement->Execute({5, 6}));

  db->PrepareStatement(
        db_factory->CreateQueryBuilder()->BuildCreateTableIfNotExistsQuery(
            kSymbolPriceTableDefinition))
      ->Execute();

  auto insert_symbol_price_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildInsertQuery(
          kSymbolPriceTableDefinition));
  insert_symbol_price_statement->Execute(
      {1, stonks::utils::GetUnixTime().count(), 12345});
  insert_symbol_price_statement->Execute(
      {2, stonks::utils::GetUnixTime().count(), 0.12345});
}

TEST(SqliteDb, SelectJoin) {
  const auto cell_definitions = std::vector<stonks::sqldb::CellDefinition>{
      stonks::sqldb::CellDefinition{
          .column = "base_asset",
          .data_type = stonks::sqldb::DataType::kString},
      stonks::sqldb::CellDefinition{
          .column = "quote_asset",
          .data_type = stonks::sqldb::DataType::kString}};

  auto select_statement = db->PrepareStatement(
      "SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS quote_asset "
      "FROM Symbol "
      "JOIN Asset AS BaseAsset ON Symbol.base_asset_id=BaseAsset.id "
      "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id=QuoteAsset.id;",
      cell_definitions);
  const auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 2);
  EXPECT_EQ(rows.GetValues("base_asset")[0].GetString(), "BTC");
  EXPECT_EQ(rows.GetValues("quote_asset")[0].GetString(), "USDT");
  EXPECT_EQ(rows.GetValues("base_asset")[1].GetString(), "ETH");
  EXPECT_EQ(rows.GetValues("quote_asset")[1].GetString(), "USDT");
}

TEST(SqliteDb, FileWriteAndRead) {
  EXPECT_FALSE(std::filesystem::exists(kTestDbFileName));
  db->WriteToFile(kTestDbFileName);
  EXPECT_TRUE(std::filesystem::exists(kTestDbFileName));

  auto db_copy = db_factory->LoadDbFromFile(kTestDbFileName);

  const auto select_query = db_factory->CreateQueryBuilder()->BuildSelectQuery(
      kSymbolPriceTableDefinition);
  const auto db_rows =
      db_copy->PrepareStatement(select_query, kSymbolPriceTableDefinition)
          ->Execute();
  const auto db_copy_rows =
      db_copy->PrepareStatement(select_query, kSymbolPriceTableDefinition)
          ->Execute();
  EXPECT_GT(db_rows.GetSize(), 0);
  EXPECT_EQ(db_rows, db_copy_rows);

  db_copy.reset();
  EXPECT_TRUE(std::filesystem::exists(kTestDbFileName));
}

TEST(SqliteDb, CascadeForeignKeyDelete) {
  db
      ->PrepareStatement(db_factory->CreateQueryBuilder()->BuildDeleteQuery(
          kAssetTableDefinition.table, "WHERE Asset.name = \"USDT\""))
      ->Execute();

  auto select_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildSelectQuery(
                               kAssetTableDefinition.table),
                           kAssetTableDefinition);
  auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 2);

  select_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildSelectQuery(
                               kSymbolTableDefinition.table),
                           kSymbolTableDefinition);
  rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 0);

  select_statement =
      db->PrepareStatement(db_factory->CreateQueryBuilder()->BuildSelectQuery(
                               kSymbolPriceTableDefinition.table),
                           kSymbolPriceTableDefinition);
  rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 0);
}
}  // namespace