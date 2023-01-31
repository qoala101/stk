#ifndef VH_STK_CORE_SDB_PROXY_H_
#define VH_STK_CORE_SDB_PROXY_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_not_null.h"
#include "cpp_optional.h"

namespace vh::stk::core::sdb {
/**
 * @brief No-op proxy to Symbols DB.
 */
class Proxy : public ISymbolsDb {
 public:
  explicit Proxy(cpp::NnUp<ISymbolsDb> symbols_db);

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
      [[nodiscard]] (const Symbol &symbol, const cpp::Opt<TimeOrder> &order,
                     const cpp::Opt<absl::Time> &start_time,
                     const cpp::Opt<absl::Time> &end_time,
                     const cpp::Opt<int> &limit) const
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
      [[nodiscard]] (const cpp::Opt<absl::Time> &start_time,
                     const cpp::Opt<absl::Time> &end_time)
      -> cppcoro::task<> override;

 private:
  cpp::NnUp<ISymbolsDb> symbols_db_;
};
}  // namespace vh::stk::core::sdb

#endif  // VH_STK_CORE_SDB_PROXY_H_
