#ifndef STONKS_STONKS_DB_STONKS_PREPARED_STATEMENTS_H_
#define STONKS_STONKS_DB_STONKS_PREPARED_STATEMENTS_H_

#include <memory>

#include "not_null.hpp"
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
  explicit PreparedStatements(
      cpp::not_null<std::shared_ptr<sqldb::IDb>> db,
      cpp::not_null<std::shared_ptr<sqldb::IQueryBuilder>> query_builder);

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
  cpp::not_null<std::shared_ptr<sqldb::IDb>> db_;
  cpp::not_null<std::shared_ptr<sqldb::IQueryBuilder>> query_builder_;
  cpp::not_null<std::unique_ptr<sqldb::QueryBuilderFacade>>
      query_builder_facade_;

  mutable std::unique_ptr<sqldb::ISelectStatement> select_assets_;
  mutable std::unique_ptr<sqldb::ISelectStatement> select_assets_with_ids_;
  mutable std::unique_ptr<sqldb::IUpdateStatement> delete_asset_;
  mutable std::unique_ptr<sqldb::IUpdateStatement> insert_asset_;
  mutable std::unique_ptr<sqldb::ISelectStatement> select_symbols_;
  mutable std::unique_ptr<sqldb::ISelectStatement> select_symbols_with_ids_;
  mutable std::unique_ptr<sqldb::ISelectStatement> select_symbols_info_;
  mutable std::unique_ptr<sqldb::IUpdateStatement> insert_symbol_info_;
  mutable std::unique_ptr<sqldb::IUpdateStatement> update_symbol_info_;
  mutable std::unique_ptr<sqldb::IUpdateStatement> delete_symbol_info_;
  mutable std::unique_ptr<sqldb::ISelectStatement> select_price_ticks_;
  mutable std::unique_ptr<sqldb::ISelectStatement> select_symbol_price_ticks_;
  mutable std::unique_ptr<sqldb::IUpdateStatement> insert_price_tick_;
};
}  // namespace stonks::db

#endif  // STONKS_STONKS_DB_STONKS_PREPARED_STATEMENTS_H_
