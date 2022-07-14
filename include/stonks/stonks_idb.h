#ifndef STONKS_STONKS_STONKS_IDB_H_
#define STONKS_STONKS_STONKS_IDB_H_

#include <string>
#include <vector>

#include "stonks_types.h"

namespace stonks {
/**
 * @brief Main application DB.
 * Used to store info about symbols, strategies, and orders.
 */
// NOLINTNEXTLINE(*-special-member-functions)
class IDb {
 public:
  virtual ~IDb() = default;

  /**
   * @brief Selects all assets.
   */
  [[nodiscard]] virtual auto SelectAssets() const
      -> std::vector<std::string> = 0;

  /**
   * @brief Updates Assets table with the new list of assets.
   */
  virtual void UpdateAssets(std::vector<std::string> assets) = 0;

  /**
   * @brief Selects all symbols.
   */
  [[nodiscard]] virtual auto SelectSymbols() const
      -> std::vector<SymbolName> = 0;

  /**
   * @brief Selects all symbols info.
   */
  [[nodiscard]] virtual auto SelectSymbolsInfo() const
      -> std::vector<SymbolInfo> = 0;

  /**
   * @brief Updates Symbols table with the new list of symbols info.
   */
  virtual void UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) = 0;

  /**
   * @brief Selects price ticks following the conditions.
   */
  [[nodiscard]] virtual auto SelectSymbolPriceTicks(const SymbolName *symbol,
                                                    const Period *period,
                                                    const int *limit) const
      -> std::vector<SymbolPriceTick> = 0;

  /**
   * @brief Inserts value into the table.
   */
  virtual void InsertSymbolPriceTick(
      const SymbolPriceTick &symbol_price_tick) = 0;
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_IDB_H_
