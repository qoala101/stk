#ifndef STONKS_CLIENTS_CLIENT_FINANCE_DB_H_
#define STONKS_CLIENTS_CLIENT_FINANCE_DB_H_

#include <optional>
#include <string>
#include <vector>

#include "client.h"
#include "finance_types.h"
#include "stonks_db.h"

namespace stonks::client {
class FinanceDb : public StonksDb {
 public:
  explicit FinanceDb(int port);

  [[nodiscard]] auto SelectAssets() const -> std::vector<std::string> override;
  void UpdateAssets(std::vector<std::string> assets) override;
  [[nodiscard]] auto SelectSymbols() const
      -> std::vector<finance::SymbolName> override;
  [[nodiscard]] auto SelectSymbolsInfo() const
      -> std::vector<finance::SymbolInfo> override;
  void UpdateSymbolsInfo(
      std::vector<finance::SymbolInfo> symbols_info) override;
  [[nodiscard]] auto SelectSymbolPriceTicks(const finance::SymbolName *symbol,
                                            const finance::Period *period,
                                            const int *limit) const
      -> std::vector<finance::SymbolPriceTick> override;
  void InsertSymbolPriceTick(
      const finance::SymbolPriceTick &symbol_price_tick) override;

 private:
  network::Client client_;
};
}  // namespace stonks::client

#endif  // STONKS_CLIENTS_CLIENT_FINANCE_DB_H_
