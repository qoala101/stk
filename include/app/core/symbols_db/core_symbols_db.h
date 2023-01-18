#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_meta_thread_safe.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
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
  explicit SymbolsDb(cpp::meta::ThreadSafe<cpp::NnUp<sqldb::IDb>> sql_db);

  /**
   * @copydoc ISymbolsDb::SelectAssets
   */
  auto SelectAssets [[nodiscard]] () const
      -> cppcoro::task<std::vector<Asset>> override;

  /**
   * @copydoc ISymbolsDb::UpdateAssets
   */
  auto UpdateAssets [[nodiscard]] (std::vector<Asset> assets)
  -> cppcoro::task<> override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolsWithPriceRecords
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> cppcoro::task<std::vector<Symbol>> override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolInfo
   */
  auto SelectSymbolInfo [[nodiscard]] (Symbol symbol) const
      -> cppcoro::task<cpp::Opt<SymbolInfo>> override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolsInfo
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> cppcoro::task<std::vector<SymbolInfo>> override;

  /**
   * @copydoc ISymbolsDb::UpdateSymbolsInfo
   */
  auto UpdateSymbolsInfo [[nodiscard]] (std::vector<SymbolInfo> infos)
  -> cppcoro::task<> override;

  /**
   * @copydoc ISymbolsDb::SelectSymbolPriceRecords
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const Symbol &symbol, const TimeOrder *order,
                     const absl::Time *start_time, const absl::Time *end_time,
                     const int *limit) const
      -> cppcoro::task<std::vector<SymbolPriceRecord>> override;

  /**
   * @copydoc ISymbolsDb::InsertSymbolPriceRecord
   */
  auto InsertSymbolPriceRecord [[nodiscard]] (SymbolPriceRecord record)
  -> cppcoro::task<> override;

  /**
   * @copydoc ISymbolsDb::DeleteSymbolPriceRecords
   */
  auto DeleteSymbolPriceRecords
      [[nodiscard]] (const absl::Time *start_time, const absl::Time *end_time)
      -> cppcoro::task<> override;

 private:
  auto SelectAssetsImpl [[nodiscard]] () const;

  void InsertAsset(Asset asset) const;
  void DeleteAsset(Asset asset) const;

  auto SelectSymbolsInfoImpl [[nodiscard]] () const;
  void InsertSymbolInfo(SymbolInfo info) const;
  void UpdateSymbolInfo(SymbolInfo info) const;
  void DeleteSymbolInfo(SymbolInfo info) const;

  struct PreparedStatements {
    cpp::NnUp<sqldb::ISelectStatement> select_assets;
    cpp::NnUp<sqldb::IUpdateStatement> insert_asset;
    cpp::NnUp<sqldb::IUpdateStatement> delete_asset;

    cpp::NnUp<sqldb::ISelectStatement> select_symbols_with_price_records;

    cpp::NnUp<sqldb::ISelectStatement> select_symbol_info;
    cpp::NnUp<sqldb::ISelectStatement> select_symbols_info;
    cpp::NnUp<sqldb::IUpdateStatement> insert_symbol_info;
    cpp::NnUp<sqldb::IUpdateStatement> update_symbol_info;
    cpp::NnUp<sqldb::IUpdateStatement> delete_symbol_info;

    struct SelectSymbolPriceRecords {
      cpp::NnUp<sqldb::ISelectStatement> order_by_time_ascending;
      cpp::NnUp<sqldb::ISelectStatement> order_by_time_descending;
    } select_symbol_price_records;

    cpp::NnUp<sqldb::IUpdateStatement> insert_symbol_price_record;
    cpp::NnUp<sqldb::IUpdateStatement> delete_symbol_price_records;
  } prepared_statements_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_
