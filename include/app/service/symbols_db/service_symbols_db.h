#ifndef STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_H_
#define STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_i_symbols_db.h"
#include "core_types.h"
#include "cpp_optional.h"
#include "service_client_base.h"
#include "service_sdb_traits.h"  // IWYU pragma: keep

namespace stonks::service {
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
      [[nodiscard]] (const core::Symbol &symbol, const core::TimeOrder *order,
                     const absl::Time *start_time, const absl::Time *end_time,
                     const int *limit) const
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
      [[nodiscard]] (const absl::Time *start_time, const absl::Time *end_time)
      -> cppcoro::task<> override;
};
}  // namespace stonks::service

#endif  // STONKS_APP_SERVICE_SYMBOLS_DB_SERVICE_SYMBOLS_DB_H_
