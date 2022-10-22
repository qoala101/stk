#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <not_null.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include "cpp_not_null.h"
#include "sqldb_as_values.h"
#include "sqldb_i_db.h"
#include "sqldb_qb_common.h"
#include "sqldb_query_builder.h"
#include "sqldb_types.h"
#include "test_sqlite_injector.h"

namespace {
auto db = stonks::cpp::Up<stonks::sqldb::IDb>{};

struct TestTable {
  struct IntTrue {
    using Table = TestTable;
    using DataType = int;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct TextFalse {
    using Table = TestTable;
    using DataType = std::string;
  };

  using Columns = std::tuple<IntTrue, TextFalse>;
};

struct Asset {
  struct id {
    using Table = Asset;
    using DataType = int64_t;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name {
    using Table = Asset;
    using DataType = std::string;
    struct Unique;
  };

  using Columns = std::tuple<id, name>;
};

TEST(SqliteDb, CreateAndDropTable) {
  const auto db_file_name =
      test::sqlite::Injector().create<stonks::sqlite::FilePath>().value;
  std::ignore = std::filesystem::remove(db_file_name);
  db = test::sqlite::Injector().create<stonks::cpp::Up<stonks::sqldb::IDb>>();

  db->PrepareStatement(stonks::sqldb::query_builder::CreateTable<TestTable>()
                           .IfNotExists()
                           .Build())
      ->Execute();
  db->PrepareStatement(stonks::sqldb::query_builder::CreateTable<TestTable>()
                           .IfNotExists()
                           .Build())
      ->Execute();
  db->PrepareStatement(
        stonks::sqldb::query_builder::DropTable<TestTable>().Build())
      ->Execute();
  EXPECT_ANY_THROW(
      db->PrepareStatement(
            stonks::sqldb::query_builder::DropTable<TestTable>().Build())
          ->Execute());
}

TEST(SqliteDb, InsertAndSelect) {
  db->PrepareStatement(stonks::sqldb::query_builder::CreateTable<Asset>()
                           .IfNotExists()
                           .Build())
      ->Execute();

  auto insert_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::Insert()
          .Value<Asset::name>(stonks::sqldb::qb::Param{}.text_)
          .Into<Asset>()
          .Build());
  insert_statement->Execute(stonks::sqldb::AsValues("BTC"));
  insert_statement->Execute(stonks::sqldb::AsValues("ETH"));
  insert_statement->Execute(stonks::sqldb::AsValues("USDT"));

  auto select_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::SelectAll().From<Asset>().Build());

  const auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 3);
  EXPECT_EQ(rows.GetColumnValues<Asset::id>()[0].GetInt64(), 1);
  EXPECT_EQ(rows.GetColumnValues<Asset::name>()[0].GetString(), "BTC");
  EXPECT_EQ(rows.GetColumnValues<Asset::id>()[1].GetInt64(), 2);
  EXPECT_EQ(rows.GetColumnValues<Asset::name>()[1].GetString(), "ETH");
  EXPECT_EQ(rows.GetColumnValues<Asset::id>()[2].GetInt64(), 3);
  EXPECT_EQ(rows.GetColumnValues<Asset::name>()[2].GetString(), "USDT");
}

TEST(SqliteDb, InsertNull) {
  auto insert_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::InsertAll().Into<Asset>().Build());
  EXPECT_ANY_THROW(insert_statement->Execute());
}

struct Symbol {
  struct id {
    using Table = Symbol;
    using DataType = int64_t;
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct base_asset_id {
    using Table = Symbol;
    using DataType = int64_t;
    using ForeignKey = Asset::id;
  };

  struct quote_asset_id {
    using Table = Symbol;
    using DataType = int64_t;
    using ForeignKey = Asset::id;
  };

  using Columns = std::tuple<id, base_asset_id, quote_asset_id>;
};

struct SymbolPrice {
  struct symbol_id {
    using Table = SymbolPrice;
    using DataType = int64_t;
    using ForeignKey = Symbol::id;
  };

  struct time {
    using Table = SymbolPrice;
    using DataType = int64_t;
  };

  struct price {
    using Table = SymbolPrice;
    using DataType = double;
  };

  using Columns = std::tuple<symbol_id, time, price>;
};

TEST(SqliteDb, ForeignKey) {
  db->PrepareStatement(stonks::sqldb::query_builder::CreateTable<Symbol>()
                           .IfNotExists()
                           .Build())
      ->Execute();

  auto insert_symbol_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::Insert()
          .Value<Symbol::base_asset_id>(stonks::sqldb::qb::Param{}.text_)
          .Value<Symbol::quote_asset_id>(stonks::sqldb::qb::Param{}.text_)
          .Into<Symbol>()
          .Build());
  insert_symbol_statement->Execute(stonks::sqldb::AsValues(1, 3));
  insert_symbol_statement->Execute(stonks::sqldb::AsValues(2, 3));
  EXPECT_ANY_THROW(
      insert_symbol_statement->Execute(stonks::sqldb::AsValues(5, 6)));

  db->PrepareStatement(stonks::sqldb::query_builder::CreateTable<SymbolPrice>()
                           .IfNotExists()
                           .Build())
      ->Execute();

  auto insert_symbol_price_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::InsertAll().Into<SymbolPrice>().Build());
  insert_symbol_price_statement->Execute(
      stonks::sqldb::AsValues(1, 1661787828796, 12345));
  insert_symbol_price_statement->Execute(
      stonks::sqldb::AsValues(2, 1661787828796, 0.12345));
}

TEST(SqliteDb, SelectJoin) {
  const auto cell_definitions = std::vector<stonks::sqldb::CellDefinition>{
      stonks::sqldb::CellDefinition{
          .column_name = {"base_asset"},
          .data_type = stonks::sqldb::DataType::kString},
      stonks::sqldb::CellDefinition{
          .column_name = {"quote_asset"},
          .data_type = stonks::sqldb::DataType::kString}};

  auto select_statement = db->PrepareStatement(
      {.query =
           {"SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS "
            "quote_asset "
            "FROM Symbol "
            "JOIN Asset AS BaseAsset ON Symbol.base_asset_id=BaseAsset.id "
            "JOIN Asset AS QuoteAsset ON Symbol.quote_asset_id=QuoteAsset.id;"},
       .column_types = cell_definitions});
  const auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 2);
  EXPECT_EQ(rows.GetColumnValues<struct base_asset>()[0].GetString(), "BTC");
  EXPECT_EQ(rows.GetColumnValues<struct quote_asset>()[0].GetString(), "USDT");
  EXPECT_EQ(rows.GetColumnValues<struct base_asset>()[1].GetString(), "ETH");
  EXPECT_EQ(rows.GetColumnValues<struct quote_asset>()[1].GetString(), "USDT");
}

TEST(SqliteDb, FileWriteAndRead) {
  const auto db_file_name =
      test::sqlite::Injector().create<stonks::sqlite::FilePath>().value;
  EXPECT_FALSE(std::filesystem::exists(db_file_name));
  db.reset();
  EXPECT_TRUE(std::filesystem::exists(db_file_name));

  db = test::sqlite::Injector().create<stonks::cpp::Up<stonks::sqldb::IDb>>();
  auto db_copy =
      test::sqlite::Injector().create<stonks::cpp::Up<stonks::sqldb::IDb>>();

  const auto select_query =
      stonks::sqldb::query_builder::SelectAll().From<SymbolPrice>().Build();
  const auto db_rows = db->PrepareStatement(select_query)->Execute();
  const auto db_copy_rows = db_copy->PrepareStatement(select_query)->Execute();
  EXPECT_GT(db_rows.GetSize(), 0);
  EXPECT_EQ(db_rows, db_copy_rows);

  db_copy.reset();
  EXPECT_TRUE(std::filesystem::exists(db_file_name));
}

TEST(SqliteDb, CascadeForeignKeyDelete) {
  db->PrepareStatement(
        stonks::sqldb::query_builder::DeleteFromTable<Asset>()
            .Where(stonks::sqldb::qb::Column<Asset::name>{} == "USDT")
            .Build())
      ->Execute();

  auto select_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::SelectAll().From<Asset>().Build());
  auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 2);

  select_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::SelectAll().From<Symbol>().Build());
  rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 0);

  select_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::SelectAll().From<SymbolPrice>().Build());
  rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 0);
}
}  // namespace