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

namespace stonks::core {
/**
 * @copydoc ISymbolsDb
 */
class SymbolsDb : public ISymbolsDb {
 public:
  /**
   * @param db DB handle to operate on.
   */
  explicit SymbolsDb(cpp::NnUp<sqldb::IDb> db);

  /**
   * @copydoc ISymbolsDb::SelectAssets
   */
  auto SelectAssets [[nodiscard]] () const -> std::vector<Asset> override;

  /**
   * @copydoc ISymbolsDb::UpdateAssets
   */
  void UpdateAssets(std::vector<Asset> assets) override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolsWithPriceRecords
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> std::vector<Symbol> override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolInfo
   */
  auto SelectSymbolInfo [[nodiscard]] (Symbol symbol) const
      -> cpp::Opt<SymbolInfo> override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolsInfo
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> std::vector<SymbolInfo> override;

  /**
   * @copydoc ISymbolsDb::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(std::vector<SymbolInfo> infos) override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolPriceRecords
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const SelectSymbolPriceRecordsArgs &args) const
      -> std::vector<SymbolPriceRecord> override;

  /**
   * @copydoc ISymbolsDb::InsertSymbolPriceRecord
   */
  void InsertSymbolPriceRecord(SymbolPriceRecord record) override;

  /**
   * @copydoc ISymbolsDb::DeleteSymbolPriceRecords
   */
  void DeleteSymbolPriceRecords(absl::Time before_time) override;

 private:
  void InsertAsset(Asset asset);
  void DeleteAsset(Asset asset);

  void InsertSymbolInfo(SymbolInfo info);
  void UpdateSymbolInfo(SymbolInfo info);
  void DeleteSymbolInfo(SymbolInfo info);

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
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_
