#ifndef STONKS_APP_CORE_SYMBOLS_DB_CORE_I_SYMBOLS_DB_H_
#define STONKS_APP_CORE_SYMBOLS_DB_CORE_I_SYMBOLS_DB_H_

#include <absl/time/time.h>

#include <limits>
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
  /**
   * @copydoc SelectSymbolPriceRecords
   */
  struct SelectSymbolPriceRecordsArgs {
    Symbol symbol{};
    absl::Time start_time{absl::InfinitePast()};
    absl::Time end_time{absl::InfiniteFuture()};
    int limit{std::numeric_limits<int>::max()};
  };

  virtual ~ISymbolsDb() = default;

  /**
   * @brief Selects all assets.
   */
  virtual auto SelectAssets [[nodiscard]] () const -> std::vector<Asset> = 0;

  /**
   * @brief Updates the list of all assets.
   */
  virtual void UpdateAssets(std::vector<Asset> assets) = 0;

  /**
   * @brief Selects symbols which have price records.
   */
  virtual auto SelectSymbolsWithPriceRecords [[nodiscard]] () const
      -> std::vector<Symbol> = 0;

  /**
   * @brief Selects symbol info.
   * @return Nullopt if symbol doesn't exist.
   */
  virtual auto SelectSymbolInfo [[nodiscard]] (Symbol symbol) const
      -> cpp::Opt<SymbolInfo> = 0;

  /**
   * @brief Selects all symbols info.
   */
  virtual auto SelectSymbolsInfo [[nodiscard]] () const
      -> std::vector<SymbolInfo> = 0;

  /**
   * @brief Updates the list of all symbol infos.
   */
  virtual void UpdateSymbolsInfo(std::vector<SymbolInfo> infos) = 0;

  /**
   * @brief Selects symbol price records following the conditions.
   */
  virtual auto SelectSymbolPriceRecords
      [[nodiscard]] (const SelectSymbolPriceRecordsArgs &args) const
      -> std::vector<SymbolPriceRecord> = 0;

  /**
   * @brief Records symbol price.
   */
  virtual void InsertSymbolPriceRecord(SymbolPriceRecord record) = 0;

  /**
   * @brief Deletes symbol price records older than specified time.
   */
  virtual void DeleteSymbolPriceRecords(absl::Time before_time) = 0;
};
}  // namespace stonks::core

#endif  // STONKS_APP_CORE_SYMBOLS_DB_CORE_I_SYMBOLS_DB_H_
