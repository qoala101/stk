/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_STK_SERVICE_SYMBOLS_DB_H_
#define VH_STK_SERVICE_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_optional.h"
#include "service_client_base.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep

namespace vh::stk::service {
/**
 * @copydoc core::ISymbolsDb
 */
class SymbolsDb : public ClientBase<core::ISymbolsDb> {
 public:
  using ClientBase::ClientBase;

  /**
   * @copydoc core::ISymbolsDb::SelectAssets
   */
  auto SelectAssets [[nodiscard]] () const
      -> cppcoro::task<std::vector<core::Asset>> override;

  /**
   * @copydoc core::ISymbolsDb::UpdateAssets
   */
  auto UpdateAssets [[nodiscard]] (std::vector<core::Asset> assets)
  -> cppcoro::task<> override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolsWithPriceRecords
   */
  auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> cppcoro::task<std::vector<core::Symbol>> override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolInfo
   */
  auto SelectSymbolInfo [[nodiscard]] (core::Symbol symbol) const
      -> cppcoro::task<cpp::Opt<core::SymbolInfo>> override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolsInfo
   */
  auto SelectSymbolsInfo [[nodiscard]] () const
      -> cppcoro::task<std::vector<core::SymbolInfo>> override;

  /**
   * @copydoc core::ISymbolsDb::UpdateSymbolsInfo
   */
  auto UpdateSymbolsInfo [[nodiscard]] (std::vector<core::SymbolInfo> infos)
  -> cppcoro::task<> override;

  /**
   * @copydoc core::ISymbolsDb::SelectSymbolPriceRecords
   */
  auto SelectSymbolPriceRecords
      [[nodiscard]] (const core::Symbol &symbol,
                     const cpp::Opt<core::TimeOrder> &order,
                     const cpp::Opt<absl::Time> &start_time,
                     const cpp::Opt<absl::Time> &end_time,
                     const cpp::Opt<int> &limit) const
      -> cppcoro::task<std::vector<core::SymbolPriceRecord>> override;

  /**
   * @copydoc core::ISymbolsDb::InsertSymbolPriceRecord
   */
  auto InsertSymbolPriceRecord [[nodiscard]] (core::SymbolPriceRecord record)
  -> cppcoro::task<> override;

  /**
   * @copydoc core::ISymbolsDb::DeleteSymbolPriceRecords
   */
  auto DeleteSymbolPriceRecords
      [[nodiscard]] (const cpp::Opt<absl::Time> &start_time,
                     const cpp::Opt<absl::Time> &end_time)
      -> cppcoro::task<> override;
};
}  // namespace vh::stk::service

#endif  // VH_STK_SERVICE_SYMBOLS_DB_H_
