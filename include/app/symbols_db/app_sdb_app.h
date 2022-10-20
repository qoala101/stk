#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_

#include <absl/time/time.h>

#include <limits>
#include <vector>

#include "app_sdb_prepared_statements.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
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
  App(cpp::NnUp<sqldb::IDb> db, cpp::NnUp<sqldb::IQueryBuilder> query_builder);

  /**
   * @brief Selects all assets.
   */
  [[nodiscard]] auto SelectAssets() const -> std::vector<core::Asset>;

  /**
   * @brief Updates the list of all assets.
   */
  void UpdateAssets(std::vector<core::Asset> assets);

  /**
   * @brief Selects symbols which have price records.
   */
  [[nodiscard]] auto SelectSymbolsWithPriceRecords() const
      -> std::vector<core::Symbol>;

  /**
   * @brief Selects symbol info.
   * @return Nullopt if symbol doesn't exist.
   */
  [[nodiscard]] auto SelectSymbolInfo(core::Symbol symbol) const
      -> cpp::Opt<core::SymbolInfo>;

  /**
   * @brief Selects all symbols info.
   */
  [[nodiscard]] auto SelectSymbolsInfo() const -> std::vector<core::SymbolInfo>;

  /**
   * @brief Updates the list of all symbol infos.
   */
  void UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos);

  /**
   * @copydoc SelectSymbolPriceRecords
   */
  struct SelectSymbolPriceRecordsArgs {
    core::Symbol symbol{};
    absl::Time start_time{absl::InfinitePast()};
    absl::Time end_time{absl::InfiniteFuture()};
  };

  /**
   * @brief Selects symbol price records following the conditions.
   */
  [[nodiscard]] auto SelectSymbolPriceRecords(
      const SelectSymbolPriceRecordsArgs &args) const
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

  void InsertAsset(core::Asset asset);
  void DeleteAsset(core::Asset asset);

  void InsertSymbolInfo(core::SymbolInfo info);
  void UpdateSymbolInfo(core::SymbolInfo info);
  void DeleteSymbolInfo(core::SymbolInfo info);

  cpp::NnSp<sqldb::IDb> db_;
  cpp::NnUp<sqldb::IQueryBuilder> query_builder_;
  PreparedStatements prepared_statements_;
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
