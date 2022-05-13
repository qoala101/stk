#ifndef STONKS_FINANCE_DB_FINANCE_DB_H_
#define STONKS_FINANCE_DB_FINANCE_DB_H_

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "finance_types.h"

namespace stonks::finance {
class FinanceDb {
 public:
  /**
   * @brief Creates handle to the DB on specified URI.
   * @remark Would create URI if it doesn't exist.
   * @remark If DB cannot be instantiated with such URI, it would use null-DB
   * on which all the calls fail.
   */
  explicit FinanceDb(std::string_view uri = "stonks.db");

  explicit FinanceDb(const FinanceDb &) = delete;
  auto operator=(const FinanceDb &) -> FinanceDb & = delete;

  FinanceDb(FinanceDb &&) = default;
  auto operator=(FinanceDb &&) -> FinanceDb & = default;

  /**
   * @brief Releases DB handle.
   */
  ~FinanceDb();

  /**
   * @brief Selects all assets.
   * @return Nullopt if operation failed.
   */
  [[nodiscard]] auto SelectAssets() const
      -> std::optional<std::vector<std::string>>;

  /**
   * @brief Selects all symbols.
   * @return Nullopt if operation failed.
   */
  [[nodiscard]] auto SelectSymbols() const
      -> std::optional<std::vector<Symbol>>;

  /**
   * @brief Inserts value into the table.
   * @return False if operation failed.
   */
  [[nodiscard]] auto InsertSymbolPriceTick(
      const SymbolPriceTick &symbol_price_tick) -> bool;

  /**
   * @brief Selects all price ticks for symbols in period.
   * @return Nullopt if operation failed.
   */
  [[nodiscard]] auto SelectSymbolPriceTicks(
      const std::optional<Period> &period = std::nullopt,
      const std::optional<std::vector<Symbol>> &symbols = std::nullopt) const
      -> std::optional<std::vector<SymbolPriceTick>>;

  [[nodiscard]] auto SelectSymbolById(int64_t symbol_id) const
      -> std::optional<Symbol>;

  [[nodiscard]] auto SelectSymbolByName(std::string_view symbol_name) const
      -> std::optional<Symbol>;

  [[nodiscard]] auto SelectSymbolIdBySymbol(const Symbol &symbol) const
      -> std::optional<int64_t>;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_FINANCE_DB_H_
