#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_PREPARED_STATEMENTS_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_PREPARED_STATEMENTS_H_

#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_query_builder_facade.h"

namespace stonks::app::sdb {
/**
 * @brief Lazy valuated prepared statements.
 */
class PreparedStatements {
 public:
  PreparedStatements(cpp::NnSp<sqldb::IDb> db,
                     sqldb::QueryBuilderFacade query_builder_facade);

  [[nodiscard]] auto SelectAssets() const -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto InsertAsset() const -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto DeleteAsset() const -> const sqldb::IUpdateStatement &;

  [[nodiscard]] auto SelectSymbolPriceRecords() const
      -> const sqldb::ISelectStatement &;
  [[nodiscard]] auto InsertSymbolPriceRecord() const
      -> const sqldb::IUpdateStatement &;
  [[nodiscard]] auto DeleteSymbolPriceRecords() const
      -> const sqldb::IUpdateStatement &;

 private:
  cpp::NnSp<sqldb::IDb> db_;
  sqldb::QueryBuilderFacade query_builder_facade_;

  mutable cpp::Up<sqldb::ISelectStatement> select_assets_{};
  mutable cpp::Up<sqldb::IUpdateStatement> insert_asset_{};
  mutable cpp::Up<sqldb::IUpdateStatement> delete_asset_{};

  mutable cpp::Up<sqldb::ISelectStatement> select_symbol_price_records_{};
  mutable cpp::Up<sqldb::IUpdateStatement> insert_symbol_price_record_{};
  mutable cpp::Up<sqldb::IUpdateStatement> delete_symbol_price_records_{};
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_PREPARED_STATEMENTS_H_
