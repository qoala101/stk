#ifndef STONKS_FINANCE_DB_FINANCE_DB_H_
#define STONKS_FINANCE_DB_FINANCE_DB_H_

#include <concepts>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "finance_types.h"
#include "stonks_db.h"

namespace stonks::finance {
class FinanceDb : public StonksDb {
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
  ~FinanceDb() override;

  /**
   * @copydoc StonksDb::SelectAssets
   */
  [[nodiscard]] auto SelectAssets() const -> std::vector<std::string> override;

  /**
   * @copydoc StonksDb::UpdateAssets
   */
  void UpdateAssets(std::vector<std::string> assets) override;

  /**
   * @copydoc StonksDb::SelectSymbols
   */
  [[nodiscard]] auto SelectSymbols() const -> std::vector<SymbolName> override;

  /**
   * @copydoc StonksDb::SelectSymbolsInfo
   */
  [[nodiscard]] auto SelectSymbolsInfo() const
      -> std::vector<SymbolInfo> override;

  /**
   * @copydoc StonksDb::UpdateSymbolsInfo
   */
  void UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) override;

  /**
   * @copydoc StonksDb::SelectSymbolPriceTicks
   */
  [[nodiscard]] auto SelectSymbolPriceTicks(
      std::optional<int> limit, const std::optional<Period> &period,
      const std::optional<std::vector<SymbolName>> &symbols) const
      -> std::vector<SymbolPriceTick> override;

  /**
   * @copydoc StonksDb::InsertSymbolPriceTick
   */
  void InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_FINANCE_DB_H_
