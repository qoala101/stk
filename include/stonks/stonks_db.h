#ifndef STONKS_STONKS_STONKS_DB_H_
#define STONKS_STONKS_STONKS_DB_H_

#include <string>
#include <vector>

#include "finance_types.h"

namespace stonks {
class StonksDb {  // NOLINT(*-special-member-functions)
 public:
  virtual ~StonksDb() = default;

  /**
   * @brief Selects all assets.
   * @throws If operation failed.
   */
  [[nodiscard]] virtual auto SelectAssets() const
      -> std::vector<std::string> = 0;

  /**
   * @brief Updates Assets table with the new list of assets.
   * @throws If operation failed.
   */
  virtual void UpdateAssets(std::vector<std::string> assets) = 0;

  /**
   * @brief Selects all symbols.
   * @throws If operation failed.
   */
  [[nodiscard]] virtual auto SelectSymbols() const
      -> std::vector<finance::SymbolName> = 0;

  /**
   * @brief Selects all symbols info.
   * @throws If operation failed.
   */
  [[nodiscard]] virtual auto SelectSymbolsInfo() const
      -> std::vector<finance::SymbolInfo> = 0;

  /**
   * @brief Updates Symbols table with the new list of symbols info.
   * @throws If operation failed.
   */
  virtual void UpdateSymbolsInfo(
      std::vector<finance::SymbolInfo> symbols_info) = 0;

  /**
   * @brief Selects price ticks following the conditions.
   * @throws If operation failed.
   */
  [[nodiscard]] virtual auto SelectSymbolPriceTicks(
      std::optional<int> limit, const std::optional<finance::Period> &period,
      const std::optional<std::vector<finance::SymbolName>> &symbols) const
      -> std::vector<finance::SymbolPriceTick> = 0;

  /**
   * @brief Inserts value into the table.
   * @throws If operation failed.
   */
  virtual void InsertSymbolPriceTick(
      const finance::SymbolPriceTick &symbol_price_tick) = 0;
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_DB_H_
