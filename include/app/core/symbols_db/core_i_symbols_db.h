#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_I_SYMBOLS_DB_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_I_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <cppcoro/task.hpp>
#include <vector>

#include "core_types.h"
#include "cpp_optional.h"

namespace stonks::core {
/**
 * @brief SQL DB manager which provides API to operate with symbols
 * and prices related data.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class ISymbolsDb {
 public:
  virtual ~ISymbolsDb() = default;

  /**
   * @brief Selects all assets.
   */
  virtual auto SelectAssets [[nodiscard]] () const
      -> cppcoro::task<std::vector<Asset>> = 0;

  /**
   * @brief Updates the list of all assets.
   */
  virtual auto UpdateAssets [[nodiscard]] (std::vector<Asset> assets)
  -> cppcoro::task<> = 0;

  /**
   * @brief Selects symbol info.
   * @return Nullopt if symbol doesn't exist.
   */
  virtual auto SelectSymbolInfo [[nodiscard]] (Symbol symbol) const
      -> cppcoro::task<cpp::Opt<SymbolInfo>> = 0;

  /**
   * @brief Selects all symbols info.
   */
  virtual auto SelectSymbolsInfo [[nodiscard]] () const
      -> cppcoro::task<std::vector<SymbolInfo>> = 0;

  /**
   * @brief Updates the list of all symbol infos.
   */
  virtual auto UpdateSymbolsInfo [[nodiscard]] (std::vector<SymbolInfo> infos)
  -> cppcoro::task<> = 0;

  /**
   * @brief Selects symbol price records following the conditions.
   */
  virtual auto SelectSymbolPriceRecords
      [[nodiscard]] (const Symbol &symbol, const absl::Time *start_time,
                     const absl::Time *end_time, const int *limit) const
      -> cppcoro::task<std::vector<SymbolPriceRecord>> = 0;

  /**
   * @brief Records symbol price.
   */
  virtual auto InsertSymbolPriceRecord [[nodiscard]] (SymbolPriceRecord record)
  -> cppcoro::task<> = 0;

  /**
   * @brief Deletes symbol price records in specified time range.
   */
  virtual auto DeleteSymbolPriceRecords
      [[nodiscard]] (const absl::Time *start_time, const absl::Time *end_time)
      -> cppcoro::task<> = 0;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_I_SYMBOLS_DB_H_
