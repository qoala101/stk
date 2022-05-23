#ifndef STONKS_FINANCE_DB_FINANCE_DB_H_
#define STONKS_FINANCE_DB_FINANCE_DB_H_

#include <concepts>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "finance_types.h"

namespace stonks::finance {
// TODO(vh): make finance db methods throw instead of optional return?
class FinanceDb {
 public:
  /**
   * @brief Creates handle to the DB on specified URI.
   * @remark Would create URI if it doesn't exist.
   * @remark If DB cannot be instantiated with such URI, it would use null-DB
   * on which all the calls fail.
   */
  explicit FinanceDb(std::string_view uri = "stonks.db");

  FinanceDb(const FinanceDb &) = delete;
  FinanceDb(FinanceDb &&) noexcept;

  auto operator=(const FinanceDb &) -> FinanceDb & = delete;
  auto operator=(FinanceDb &&) noexcept -> FinanceDb &;

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
      std::optional<int> limit = std::nullopt,
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

template <typename T>
concept FinanceDbConcept = requires(const T &t) {
  { t.SelectAssets() } -> std::same_as<std::optional<std::vector<std::string>>>;
}
&&requires(const T &t) {
  { t.SelectSymbols() } -> std::same_as<std::optional<std::vector<Symbol>>>;
}
&&requires(T &t, const SymbolPriceTick &symbol_price_tick) {
  { t.InsertSymbolPriceTick(symbol_price_tick) } -> std::same_as<bool>;
}
&&requires(T &t, std::optional<int> limit, const std::optional<Period> &period,
           const std::optional<std::vector<Symbol>> &symbols) {
  {
    t.SelectSymbolPriceTicks(limit, period, symbols)
    } -> std::same_as<std::optional<std::vector<SymbolPriceTick>>>;
};

static_assert(FinanceDbConcept<FinanceDb>);
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_FINANCE_DB_H_
