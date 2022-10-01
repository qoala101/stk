#ifndef STONKS_STONKS_DB_STONKS_DB_PREPARED_STATEMENTS_H_
#define STONKS_STONKS_DB_STONKS_DB_PREPARED_STATEMENTS_H_

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "di_factory.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_query_builder_facade.h"

namespace stonks::db {
/**
 * @brief Storage of prepared statements used by DB.
 * @remark Statements are prepared when requested first time.
 */
class PreparedStatements {
 public:
  explicit PreparedStatements(cpp::NnSp<sqldb::IDb> db,
                              sqldb::QueryBuilderFacade query_builder_facade);

  [[nodiscard]] auto SelectAssets() const -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectAssetsWithIds() const
      -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto DeleteAsset() const -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto InsertAsset() const -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto SelectSymbols() const -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectSymbolsWithIds() const
      -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectSymbolsInfo() const
      -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto InsertSymbolInfo() const
      -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto UpdateSymbolInfo() const
      -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto DeleteSymbolInfo() const
      -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto SelectPriceTicks() const
      -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectSymbolPriceTicks() const
      -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto InsertPriceTick() const -> const sqldb::IUpdateStatement &;

 private:
  cpp::NnSp<sqldb::IDb> db_;
  sqldb::QueryBuilderFacade query_builder_facade_;

  mutable cpp::Up<sqldb::ISelectStatement> select_assets_;
  mutable cpp::Up<sqldb::ISelectStatement> select_assets_with_ids_;
  mutable cpp::Up<sqldb::IUpdateStatement> delete_asset_;
  mutable cpp::Up<sqldb::IUpdateStatement> insert_asset_;
  mutable cpp::Up<sqldb::ISelectStatement> select_symbols_;
  mutable cpp::Up<sqldb::ISelectStatement> select_symbols_with_ids_;
  mutable cpp::Up<sqldb::ISelectStatement> select_symbols_info_;
  mutable cpp::Up<sqldb::IUpdateStatement> insert_symbol_info_;
  mutable cpp::Up<sqldb::IUpdateStatement> update_symbol_info_;
  mutable cpp::Up<sqldb::IUpdateStatement> delete_symbol_info_;
  mutable cpp::Up<sqldb::ISelectStatement> select_price_ticks_;
  mutable cpp::Up<sqldb::ISelectStatement> select_symbol_price_ticks_;
  mutable cpp::Up<sqldb::IUpdateStatement> insert_price_tick_;
};
}  // namespace stonks::db

#endif  // STONKS_STONKS_DB_STONKS_DB_PREPARED_STATEMENTS_H_
