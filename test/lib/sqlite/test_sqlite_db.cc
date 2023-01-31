#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include <boost/di.hpp>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <not_null.hpp>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "cpp_type_list.h"
#include "cpp_typed_struct.h"
#include "gtest/gtest_pred_impl.h"
#include "sqldb_alias_to_table.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_prm_types.h"
#include "sqldb_qb_common.h"
#include "sqldb_qb_create.h"
#include "sqldb_qb_delete.h"
#include "sqldb_qb_drop.h"
#include "sqldb_qb_insert.h"
#include "sqldb_qb_query_value.h"
#include "sqldb_qb_select.h"
#include "sqldb_query_builder.h"
#include "sqldb_rows.h"
#include "sqldb_table.h"
#include "sqldb_types.h"
#include "sqldb_value.h"
#include "sqldb_value_conversions.h"
#include "sqlite_types.h"
#include "test_sqlite_injector.h"

namespace {
auto db = vh::cpp::Up<vh::sqldb::IDb>{};

struct TestTable : public vh::sqldb::Table<TestTable> {
  struct IntTrue : public Column<int, IntTrue> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct TextFalse : public Column<std::string, TextFalse> {};

  using Columns = vh::cpp::TypeList<IntTrue, TextFalse>;
};

struct Asset : public vh::sqldb::Table<Asset> {
  struct id : public Column<int64_t, id> {
    struct PrimaryKey;
    struct AutoIncrement;
    struct Unique;
  };

  struct name : public Column<std::string, name> {
    struct Unique;
  };

  using Columns = vh::cpp::TypeList<id, name>;
};

TEST(SqliteDb, CreateAndDropTable) {
  const auto db_file_name =
      *test::sqlite::Injector().create<vh::sqlite::FilePath>();
  std::ignore = std::filesystem::remove(db_file_name);
  db = test::sqlite::Injector().create<vh::cpp::Up<vh::sqldb::IDb>>();

  db->PrepareStatement(vh::sqldb::query_builder::CreateTable<TestTable>()
                           .IfNotExists()
                           .Build())
      ->Execute();
  db->PrepareStatement(vh::sqldb::query_builder::CreateTable<TestTable>()
                           .IfNotExists()
                           .Build())
      ->Execute();
  db->PrepareStatement(vh::sqldb::query_builder::DropTable<TestTable>().Build())
      ->Execute();
  EXPECT_ANY_THROW(
      db->PrepareStatement(
            vh::sqldb::query_builder::DropTable<TestTable>().Build())
          ->Execute());
}

TEST(SqliteDb, InsertAndSelect) {
  db->PrepareStatement(
        vh::sqldb::query_builder::CreateTable<Asset>().IfNotExists().Build())
      ->Execute();

  auto insert_statement =
      db->PrepareStatement(vh::sqldb::query_builder::Insert()
                               .Value<Asset::name>(vh::sqldb::prm::QueryParam{})
                               .Into<Asset>()
                               .Build());
  insert_statement->Execute(vh::sqldb::AsValues("BTC"));
  insert_statement->Execute(vh::sqldb::AsValues("ETH"));
  insert_statement->Execute(vh::sqldb::AsValues("USDT"));

  auto select_statement = db->PrepareStatement(
      vh::sqldb::query_builder::SelectAll().From<Asset>().Build());

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
      vh::sqldb::query_builder::InsertAll().Into<Asset>().Build());
  EXPECT_ANY_THROW(insert_statement->Execute());
}

struct Symbol : public vh::sqldb::Table<Symbol> {
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

  using Columns = vh::cpp::TypeList<id, base_asset_id, quote_asset_id>;
};

struct SymbolPrice : public vh::sqldb::Table<SymbolPrice> {
  struct symbol_id : public Column<int64_t, symbol_id> {
    using ForeignKey = Symbol::id;
  };

  struct time : public Column<int64_t, time> {};

  struct price : public Column<int64_t, price> {};

  using Columns = vh::cpp::TypeList<symbol_id, time, price>;
};

TEST(SqliteDb, ForeignKey) {
  db->PrepareStatement(
        vh::sqldb::query_builder::CreateTable<Symbol>().IfNotExists().Build())
      ->Execute();

  auto insert_symbol_statement = db->PrepareStatement(
      vh::sqldb::query_builder::Insert()
          .Value<Symbol::base_asset_id>(vh::sqldb::prm::QueryParam{})
          .Value<Symbol::quote_asset_id>(vh::sqldb::prm::QueryParam{})
          .Into<Symbol>()
          .Build());
  insert_symbol_statement->Execute(vh::sqldb::AsValues(1, 3));
  insert_symbol_statement->Execute(vh::sqldb::AsValues(2, 3));
  EXPECT_ANY_THROW(insert_symbol_statement->Execute(vh::sqldb::AsValues(5, 6)));

  db->PrepareStatement(vh::sqldb::query_builder::CreateTable<SymbolPrice>()
                           .IfNotExists()
                           .Build())
      ->Execute();

  auto insert_symbol_price_statement = db->PrepareStatement(
      vh::sqldb::query_builder::InsertAll().Into<SymbolPrice>().Build());
  insert_symbol_price_statement->Execute(
      vh::sqldb::AsValues(1, 1661787828796, 12345));
  insert_symbol_price_statement->Execute(
      vh::sqldb::AsValues(2, 1661787828796, 0.12345));
}

struct BaseAsset : public vh::sqldb::AliasToTable<Asset, BaseAsset> {
  struct base_asset : public AliasToColumn<Target::name, base_asset> {};
};

struct QuoteAsset : public vh::sqldb::AliasToTable<Asset, QuoteAsset> {
  struct quote_asset : public AliasToColumn<Target::name, quote_asset> {};
};

TEST(SqliteDb, SelectJoin) {
  const auto cell_definitions =
      vh::sqldb::ResultDefinition{std::vector<vh::sqldb::ColumnType>{
          vh::sqldb::ColumnType{.column = {"base_asset"},
                                .type = {vh::sqldb::DataType<std::string>{}}},
          vh::sqldb::ColumnType{.column = {"quote_asset"},
                                .type = {vh::sqldb::DataType<std::string>{}}}}};

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
      *test::sqlite::Injector().create<vh::sqlite::FilePath>();
  EXPECT_FALSE(std::filesystem::exists(db_file_name));
  db.reset();
  EXPECT_TRUE(std::filesystem::exists(db_file_name));

  db = test::sqlite::Injector().create<vh::cpp::Up<vh::sqldb::IDb>>();
  auto db_copy = test::sqlite::Injector().create<vh::cpp::Up<vh::sqldb::IDb>>();

  const auto select_query =
      vh::sqldb::query_builder::SelectAll().From<SymbolPrice>().Build();
  const auto db_rows = db->PrepareStatement(select_query)->Execute();
  const auto db_copy_rows = db_copy->PrepareStatement(select_query)->Execute();
  EXPECT_GT(db_rows.GetSize(), 0);
  EXPECT_EQ(db_rows, db_copy_rows);

  db_copy.reset();
  EXPECT_TRUE(std::filesystem::exists(db_file_name));
}

TEST(SqliteDb, CascadeForeignKeyDelete) {
  db->PrepareStatement(vh::sqldb::query_builder::DeleteFromTable<Asset>()
                           .Where(vh::sqldb::qb::Column<Asset::name>() ==
                                  vh::sqldb::Value{"USDT"})
                           .Build())
      ->Execute();

  auto select_statement = db->PrepareStatement(
      vh::sqldb::query_builder::SelectAll().From<Asset>().Build());
  auto rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 2);

  select_statement = db->PrepareStatement(
      vh::sqldb::query_builder::SelectAll().From<Symbol>().Build());
  rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 0);

  select_statement = db->PrepareStatement(
      vh::sqldb::query_builder::SelectAll().From<SymbolPrice>().Build());
  rows = select_statement->Execute();
  EXPECT_EQ(rows.GetSize(), 0);
}
}  // namespace