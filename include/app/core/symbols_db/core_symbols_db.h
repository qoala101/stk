#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <vector>

#include "core_i_symbols_db.h"
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
 * @copydoc IApp
 */
class App : public IApp {
 public:
  /**
   * @param db DB handle to operate on.
   */
  explicit App(cpp::NnUp<sqldb::IDb> db);

  /**
   * @copydoc IApp::SelectAssets
   */
  auto SelectAssets [[nodiscard]] () const -> std::vector<core::Asset> override;

  /**
   * @copydoc IApp::UpdateAssets
   */
  void UpdateAssets(std::vector<core::Asset> assets) override;

  /**
   * @copydoc IApp::SelectSymbolsWithPriceRecords
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> std::vector<core::Symbol> override;

  /**
   * @copydoc IApp::SelectSymbolInfo
   */
  auto SelectSymbolInfo [[nodiscard]] (core::Symbol symbol) const
      -> cpp::Opt<core::SymbolInfo> override;

  /**
   * @copydoc IApp::SelectSymbolsInfo
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> std::vector<core::SymbolInfo> override;

  /**
   * @copydoc IApp::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos) override;

  /**
   * @copydoc IApp::SelectSymbolPriceRecords
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const SelectSymbolPriceRecordsArgs &args) const
      -> std::vector<core::SymbolPriceRecord> override;

  /**
   * @copydoc IApp::InsertSymbolPriceRecord
   */
  void InsertSymbolPriceRecord(core::SymbolPriceRecord record) override;

  /**
   * @copydoc IApp::DeleteSymbolPriceRecords
   */
  void DeleteSymbolPriceRecords(absl::Time before_time) override;

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

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_
