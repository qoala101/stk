#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_

#include <absl/time/time.h>

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

  /**
   * @brief Updates the list of all symbol infos.
   */
  void UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos);

  /**
   * @brief Selects symbol price records following the conditions.
   */
  [[nodiscard]] auto SelectSymbolPriceRecords(const core::Symbol &symbol,
                                              const absl::Time *start_time,
                                              const absl::Time *end_time,
                                              const int *limit) const
      -> std::vector<core::SymbolPriceRecord>;

  /**
   * @brief Records symbol price.
   */
  void InsertSymbolPriceRecord(core::SymbolPriceRecord record);

  /**
   * @brief Deletes symbol price records older than specified time.
   */
  void DeleteSymbolPriceRecords(absl::Time before_time);

 private:
  void CreateTablesIfNotExist();

  [[nodiscard]] auto SelectAssets() const;
  void InsertAsset(core::Asset asset);
  void DeleteAsset(core::Asset asset);

  [[nodiscard]] auto SelectSymbolsInfo() const;
  void InsertSymbolInfo(core::SymbolInfo info);
  void UpdateSymbolInfo(core::SymbolInfo info);
  void DeleteSymbolInfo(core::SymbolInfo info);

  cpp::NnSp<sqldb::IDb> db_;
  cpp::NnUp<sqldb::IQueryBuilder> query_builder_;
  PreparedStatements prepared_statements_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
