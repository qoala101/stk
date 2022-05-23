#ifndef STONKS_SERVICES_V2_FINANCE_DB_CLIENT_SERVER_H_
#define STONKS_SERVICES_V2_FINANCE_DB_CLIENT_SERVER_H_

#include "client.h"
#include "finance_db.h"
#include "server.h"

namespace stonks::finance {
class FinanceDbClient {
 public:
  explicit FinanceDbClient(std::string_view base_uri);

  /**
   * @copydoc FinanceDb::SelectAssets
   */
  [[nodiscard]] auto SelectAssets() const
      -> std::optional<std::vector<std::string>>;

  /**
   * @copydoc FinanceDb::SelectSymbols
   */
  [[nodiscard]] auto SelectSymbols() const
      -> std::optional<std::vector<Symbol>>;

  /**
   * @copydoc FinanceDb::InsertSymbolPriceTick
   */
  [[nodiscard]] auto InsertSymbolPriceTick(
      const SymbolPriceTick &symbol_price_tick) -> bool;

  /**
   * @copydoc FinanceDb::SelectSymbolPriceTicks
   */
  [[nodiscard]] auto SelectSymbolPriceTicks(
      std::optional<int> limit = std::nullopt,
      const std::optional<Period> &period = std::nullopt,
      const std::optional<std::vector<Symbol>> &symbols = std::nullopt) const
      -> std::optional<std::vector<SymbolPriceTick>>;

 private:
  network::Client client_;
};

static_assert(FinanceDbConcept<FinanceDbClient>);

class FinanceDbServer {
 public:
  explicit FinanceDbServer(std::string_view base_uri, FinanceDb finance_db);

 private:
  network::Server server_;
  FinanceDb finance_db_{};
};
}  // namespace stonks::finance

#endif  // STONKS_SERVICES_V2_FINANCE_DB_CLIENT_SERVER_H_
