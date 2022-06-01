#ifndef STONKS_CLIENTS_CLIENT_FINANCE_DB_H_
#define STONKS_CLIENTS_CLIENT_FINANCE_DB_H_

#include "client.h"
#include "stonks_db.h"

namespace stonks {
class FinanceDbClient : public StonksDb {
 public:
  explicit FinanceDbClient(std::string_view base_uri);

  [[nodiscard]] auto SelectAssets() const -> std::vector<std::string> override;
  void UpdateAssets(std::vector<std::string> assets) override;
  [[nodiscard]] auto SelectSymbols() const
      -> std::vector<finance::SymbolName> override;
  [[nodiscard]] auto SelectSymbolsInfo() const
      -> std::vector<finance::SymbolInfo> override;
  void UpdateSymbolsInfo(
      std::vector<finance::SymbolInfo> symbols_info) override;
  [[nodiscard]] auto SelectSymbolPriceTicks(
      std::optional<int> limit, const std::optional<finance::Period> &period,
      const std::optional<std::vector<finance::SymbolName>> &symbols) const
      -> std::vector<finance::SymbolPriceTick> override;
  void InsertSymbolPriceTick(
      const finance::SymbolPriceTick &symbol_price_tick) override;

 private:
  network::Client client_;
};
}  // namespace stonks

#endif  // STONKS_CLIENTS_CLIENT_FINANCE_DB_H_
