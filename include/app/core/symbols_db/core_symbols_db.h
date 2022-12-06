#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"
#include "cpp_smart_pointers.h"
#include "sqldb_i_db.h"
#include "sqldb_i_select_statement.h"
#include "sqldb_i_update_statement.h"
#include "sqldb_p_db.h"

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
      [[nodiscard]] (const Symbol &symbol, const absl::Time *start_time,
                     const absl::Time *end_time, const int *limit) const
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

  void InsertAsset(Asset asset);
  void DeleteAsset(Asset asset);

  auto SelectSymbolsInfoImpl [[nodiscard]] () const;
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
