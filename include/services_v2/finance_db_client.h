#ifndef STONKS_SERVICES_V2_FINANCE_DB_CLIENT_H_
#define STONKS_SERVICES_V2_FINANCE_DB_CLIENT_H_

#include "client.h"
#include "finance_db.h"

namespace stonks::finance {
class FinanceDbClient : public network::Client {
 public:
  explicit FinanceDbClient(std::string_view base_uri);

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
};

static_assert(FinanceDbConcept<FinanceDbClient>);
}  // namespace stonks::finance

#endif  // STONKS_SERVICES_V2_FINANCE_DB_CLIENT_H_
