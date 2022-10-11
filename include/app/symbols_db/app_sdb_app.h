#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_

#include <vector>

#include "app_sdb_prepared_statements.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "di_factory.h"
#include "sqldb_i_db.h"
#include "sqldb_i_query_builder.h"

namespace stonks::app::sdb {
/**
 * @brief SQL DB manager which provides API to operate with symbols
 * and prices related data.
 */
class App {
 public:
  App(cpp::NnUp<sqldb::IDb> db,
      di::Factory<sqldb::IQueryBuilder> query_builder_factory);

  /**
   * @brief Updates the list of all assets.
   */
  void UpdateAssets(std::vector<core::Asset> assets);

 private:
  void CreateTablesIfNotExist();

  [[nodiscard]] auto SelectAssets() const;
  void InsertAsset(core::Asset asset);
  void DeleteAsset(core::Asset asset);

  cpp::NnSp<sqldb::IDb> db_;
  cpp::NnUp<sqldb::IQueryBuilder> query_builder_;
  PreparedStatements prepared_statements_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
