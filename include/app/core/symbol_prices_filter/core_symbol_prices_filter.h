#ifndef STONKS_APP_CORE_SYMBOL_PRICES_FILTER_CORE_SYMBOL_PRICES_FILTER_H_
#define STONKS_APP_CORE_SYMBOL_PRICES_FILTER_CORE_SYMBOL_PRICES_FILTER_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_meta_thread_safe.h"

namespace stonks::core {
/**
 * @brief Filters inserted symbol prices.
 */
class SymbolPricesFilter : public ISymbolsDb {
 public:
  explicit SymbolPricesFilter(
      cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db);

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
  auto GetBaseAssetPriceStep [[nodiscard]] (const Symbol &symbol) const
      -> cppcoro::task<double>;

  auto GetLastPriceRecord [[nodiscard]] (const Symbol &symbol) const
      -> cppcoro::task<cpp::Opt<SymbolPriceRecord>>;

  cpp::NnUp<ISymbolsDb> symbols_db_;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOL_PRICES_FILTER_CORE_SYMBOL_PRICES_FILTER_H_
