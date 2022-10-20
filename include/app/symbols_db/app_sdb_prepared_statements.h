#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_PREPARED_STATEMENTS_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_PREPARED_STATEMENTS_H_

#include "cpp_lazy.h"
#include "cpp_not_null.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"

namespace stonks::app::sdb {
/**
 * @brief Lazy valuated prepared statements.
 */
struct PreparedStatements {
  cpp::Lazy<cpp::NnUp<sqldb::ISelectStatement>> select_assets;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> insert_asset;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> delete_asset;

  cpp::Lazy<cpp::NnUp<sqldb::ISelectStatement>>
      select_symbols_with_price_records;

  cpp::Lazy<cpp::NnUp<sqldb::ISelectStatement>> select_symbol_info;
  cpp::Lazy<cpp::NnUp<sqldb::ISelectStatement>> select_symbols_info;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> insert_symbol_info;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> update_symbol_info;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> delete_symbol_info;

  cpp::Lazy<cpp::NnUp<sqldb::ISelectStatement>> select_symbol_price_records;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> insert_symbol_price_record;
  cpp::Lazy<cpp::NnUp<sqldb::IUpdateStatement>> delete_symbol_price_records;
};

/**
 * @brief Prepares the statements for DB.
 */
[[nodiscard]] auto PreparedStatementsFrom(const cpp::NnSp<sqldb::IDb> &db)
    -> PreparedStatements;
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_PREPARED_STATEMENTS_H_
