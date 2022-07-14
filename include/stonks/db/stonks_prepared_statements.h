#ifndef STONKS_STONKS_DB_STONKS_PREPARED_STATEMENTS_H_
#define STONKS_STONKS_DB_STONKS_PREPARED_STATEMENTS_H_

#include <memory>

#include "not_null.hpp"
#include "sqldb_db.h"
#include "sqldb_query_builder.h"
#include "sqldb_query_builder_facade.h"
#include "sqldb_select_statement.h"
#include "sqldb_update_statement.h"

namespace stonks::db {
/**
 * @brief Storage of prepared statements used by DB.
 * @remark Statements are prepared when requested first time.
 */
class PreparedStatements {
 public:
  explicit PreparedStatements(
      cpp::not_null<std::shared_ptr<sqldb::IDb>> db,
      cpp::not_null<std::shared_ptr<sqldb::IQueryBuilder>> query_builder);

  [[nodiscard]] auto SelectAssets() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectAssetsWithIds() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto DeleteAsset() -> sqldb::IUpdateStatement &;
  [[nodiscard]] auto InsertAsset() -> sqldb::IUpdateStatement &;
  [[nodiscard]] auto SelectSymbols() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectSymbolsWithIds() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectSymbolsInfo() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto InsertSymbolInfo() -> sqldb::IUpdateStatement &;
  [[nodiscard]] auto UpdateSymbolInfo() -> sqldb::IUpdateStatement &;
  [[nodiscard]] auto DeleteSymbolInfo() -> sqldb::IUpdateStatement &;
  [[nodiscard]] auto SelectPriceTicks() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto SelectSymbolPriceTicks() -> sqldb::ISelectStatement &;
  [[nodiscard]] auto InsertPriceTick() -> sqldb::IUpdateStatement &;

 private:
  cpp::not_null<std::shared_ptr<sqldb::IDb>> db_;
  cpp::not_null<std::shared_ptr<sqldb::IQueryBuilder>> query_builder_;
  cpp::not_null<std::unique_ptr<sqldb::QueryBuilderFacade>>
      query_builder_facade_;

  std::unique_ptr<sqldb::ISelectStatement> select_assets_;
  std::unique_ptr<sqldb::ISelectStatement> select_assets_with_ids_;
  std::unique_ptr<sqldb::IUpdateStatement> delete_asset_;
  std::unique_ptr<sqldb::IUpdateStatement> insert_asset_;
  std::unique_ptr<sqldb::ISelectStatement> select_symbols_;
  std::unique_ptr<sqldb::ISelectStatement> select_symbols_with_ids_;
  std::unique_ptr<sqldb::ISelectStatement> select_symbols_info_;
  std::unique_ptr<sqldb::IUpdateStatement> insert_symbol_info_;
  std::unique_ptr<sqldb::IUpdateStatement> update_symbol_info_;
  std::unique_ptr<sqldb::IUpdateStatement> delete_symbol_info_;
  std::unique_ptr<sqldb::ISelectStatement> select_price_ticks_;
  std::unique_ptr<sqldb::ISelectStatement> select_symbol_price_ticks_;
  std::unique_ptr<sqldb::IUpdateStatement> insert_price_tick_;
};
}  // namespace stonks::db

#endif  // STONKS_STONKS_DB_STONKS_PREPARED_STATEMENTS_H_
