#ifndef STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
#define STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_

#include <absl/time/time.h>

#include <limits>
#include <vector>

#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_smart_pointers.h"
#include "parametrized/sqldb_p_db.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"

namespace stonks::app::sdb {
/**
 * @brief SQL DB manager which provides API to operate with symbols
 * and prices related data.
 */
class App {
 public:
  explicit App(cpp::NnUp<sqldb::IDb> db);

  /**
   * @brief Selects all assets.
   */
  auto SelectAssets [[nodiscard]] () const -> std::vector<core::Asset>;

  /**
   * @brief Updates the list of all assets.
   */
  void UpdateAssets(std::vector<core::Asset> assets);

  /**
   * @brief Selects symbols which have price records.
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> std::vector<core::Symbol>;

  /**
   * @brief Selects symbol info.
   * @return Nullopt if symbol doesn't exist.
   */
  auto SelectSymbolInfo [[nodiscard]] (core::Symbol symbol) const
      -> cpp::Opt<core::SymbolInfo>;

  /**
   * @brief Selects all symbols info.
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> std::vector<core::SymbolInfo>;

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
    int limit{std::numeric_limits<int>::max()};
  };

  /**
   * @brief Selects symbol price records following the conditions.
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const SelectSymbolPriceRecordsArgs &args) const
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
  void InsertAsset(core::Asset asset);
  void DeleteAsset(core::Asset asset);

  void InsertSymbolInfo(core::SymbolInfo info);
  void UpdateSymbolInfo(core::SymbolInfo info);
  void DeleteSymbolInfo(core::SymbolInfo info);

  cpp::NnSp<sqldb::p::Db> db_;

  struct PreparedStatements {
    cpp::Up<sqldb::ISelectStatement> select_assets{};
    cpp::Up<sqldb::IUpdateStatement> insert_asset{};
    cpp::Up<sqldb::IUpdateStatement> delete_asset{};

    cpp::Up<sqldb::ISelectStatement> select_symbols_with_price_records{};

    cpp::Up<sqldb::ISelectStatement> select_symbol_info{};
    cpp::Up<sqldb::ISelectStatement> select_symbols_info{};
    cpp::Up<sqldb::IUpdateStatement> insert_symbol_info{};
    cpp::Up<sqldb::IUpdateStatement> update_symbol_info{};
    cpp::Up<sqldb::IUpdateStatement> delete_symbol_info{};

    cpp::Up<sqldb::ISelectStatement> select_symbol_price_records{};
    cpp::Up<sqldb::IUpdateStatement> insert_symbol_price_record{};
    cpp::Up<sqldb::IUpdateStatement> delete_symbol_price_records{};
  } mutable prepared_statements_{};
};
}  // namespace stonks::app::sdb

#endif  // STONKS_APP_SYMBOLS_DB_APP_SDB_APP_H_
