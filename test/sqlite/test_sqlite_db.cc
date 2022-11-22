#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <not_null.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>

#include "cpp_not_null.h"
#include "sqldb_alias_to_table.h"
#include "sqldb_i_db.h"
#include "sqldb_qb_common.h"
#include "sqldb_query_builder.h"
#include "sqldb_table.h"
#include "sqldb_value.h"
#include "sqldb_value_conversions.h"
#include "test_sqlite_injector.h"

namespace {
auto db = stonks::cpp::Up<stonks::sqldb::IDb>{};

struct TestTable : public stonks::sqldb::Table<TestTable> {
  struct IntTrue : public Column<int, IntTrue> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct TextFalse : public Column<std::string, TextFalse> {};

  using Columns = stonks::cpp::TypeList<IntTrue, TextFalse>;
};

struct Asset : public stonks::sqldb::Table<Asset> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name : public Column<std::string, name> {
    struct Unique;
  };

  using Columns = stonks::cpp::TypeList<id, name>;
};

TEST(SqliteDb, CreateAndDropTable) {
  const auto db_file_name =
      *test::sqlite::Injector().create<stonks::sqlite::FilePath>();
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
          .Value<Asset::name>(stonks::sqldb::p::QueryParam{})
          .Into<Asset>()
          .Build());
  insert_statement->Execute(stonks::sqldb::AsValues("BTC"));
  insert_statement->Execute(stonks::sqldb::AsValues("ETH"));
  insert_statement->Execute(stonks::sqldb::AsValues("USDT"));

  auto select_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::SelectAll().From<Asset>().Build());

  const auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 3);
  EXPECT_EQ(rows.GetColumnValues<Asset::id>()[0].Get<int64_t>(), 1);
  EXPECT_EQ(rows.GetColumnValues<Asset::name>()[0].Get<std::string>(), "BTC");
  EXPECT_EQ(rows.GetColumnValues<Asset::id>()[1].Get<int64_t>(), 2);
  EXPECT_EQ(rows.GetColumnValues<Asset::name>()[1].Get<std::string>(), "ETH");
  EXPECT_EQ(rows.GetColumnValues<Asset::id>()[2].Get<int64_t>(), 3);
  EXPECT_EQ(rows.GetColumnValues<Asset::name>()[2].Get<std::string>(), "USDT");
}

TEST(SqliteDb, InsertNull) {
  auto insert_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::InsertAll().Into<Asset>().Build());
  EXPECT_ANY_THROW(insert_statement->Execute());
}

struct Symbol : public stonks::sqldb::Table<Symbol> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct base_asset_id : public Column<int64_t, base_asset_id> {
    using ForeignKey = Asset::id;
  };

  struct quote_asset_id : public Column<int64_t, quote_asset_id> {
    using ForeignKey = Asset::id;
  };

  using Columns = stonks::cpp::TypeList<id, base_asset_id, quote_asset_id>;
};

struct SymbolPrice : public stonks::sqldb::Table<SymbolPrice> {
  struct symbol_id : public Column<int64_t, symbol_id> {
    using ForeignKey = Symbol::id;
  };

  struct time : public Column<int64_t, time> {};

  struct price : public Column<int64_t, price> {};

  using Columns = stonks::cpp::TypeList<symbol_id, time, price>;
};

TEST(SqliteDb, ForeignKey) {
  db->PrepareStatement(stonks::sqldb::query_builder::CreateTable<Symbol>()
                           .IfNotExists()
                           .Build())
      ->Execute();

  auto insert_symbol_statement = db->PrepareStatement(
      stonks::sqldb::query_builder::Insert()
          .Value<Symbol::base_asset_id>(stonks::sqldb::p::QueryParam{})
          .Value<Symbol::quote_asset_id>(stonks::sqldb::p::QueryParam{})
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

struct BaseAsset : public stonks::sqldb::AliasToTable<Asset, BaseAsset> {
  struct base_asset : public AliasToColumn<Target::name, base_asset> {};
};

struct QuoteAsset : public stonks::sqldb::AliasToTable<Asset, QuoteAsset> {
  struct quote_asset : public AliasToColumn<Target::name, quote_asset> {};
};

TEST(SqliteDb, SelectJoin) {
  const auto cell_definitions =
      stonks::sqldb::ResultDefinition{std::vector<stonks::sqldb::ColumnType>{
          stonks::sqldb::ColumnType{
              .column = {"base_asset"},
              .type = {stonks::sqldb::DataType<std::string>{}}},
          stonks::sqldb::ColumnType{
              .column = {"quote_asset"},
              .type = {stonks::sqldb::DataType<std::string>{}}}}};

  auto select_statement = db->PrepareStatement(
      {"SELECT BaseAsset.name AS base_asset, QuoteAsset.name AS "
       "quote_asset "
       "FROM Symbol "
       "JOIN Asset AS BaseAsset ON Symbol.base_asset_id=BaseAsset.id "
       "JOIN Asset AS QuoteAsset ON "
       "Symbol.quote_asset_id=QuoteAsset.id;",
       cell_definitions});
  const auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 2);
  EXPECT_EQ(rows.GetColumnValues<BaseAsset::base_asset>()[0].Get<std::string>(),
            "BTC");
  EXPECT_EQ(
      rows.GetColumnValues<QuoteAsset::quote_asset>()[0].Get<std::string>(),
      "USDT");
  EXPECT_EQ(rows.GetColumnValues<BaseAsset::base_asset>()[1].Get<std::string>(),
            "ETH");
  EXPECT_EQ(
      rows.GetColumnValues<QuoteAsset::quote_asset>()[1].Get<std::string>(),
      "USDT");
}

TEST(SqliteDb, FileWriteAndRead) {
  const auto db_file_name =
      *test::sqlite::Injector().create<stonks::sqlite::FilePath>();
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
  db->PrepareStatement(stonks::sqldb::query_builder::DeleteFromTable<Asset>()
                           .Where(stonks::sqldb::qb::Column<Asset::name>() ==
                                  stonks::sqldb::Value{"USDT"})
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