#ifndef STONKS_SERVICES_V2_FINANCE_DB_CLIENT_SERVER_H_
#define STONKS_SERVICES_V2_FINANCE_DB_CLIENT_SERVER_H_

#include <memory>

#include "client.h"
#include "server.h"
#include "stonks_db.h"

namespace stonks::finance {
class FinanceDbClient : public StonksDb {
 public:
  explicit FinanceDbClient(std::string_view base_uri);

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
      -> std::vector<finance::SymbolInfo> override;

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
   * @copydoc StonksDb::SelectAssets
   */
  void InsertSymbolPriceTick(const SymbolPriceTick &symbol_price_tick) override;

 private:
  network::Client client_;
};

class StonksDbServer {
 public:
  explicit StonksDbServer(std::string_view base_uri,
                          std::shared_ptr<StonksDb> stonks_db);

 private:
  network::Server server_;
  std::shared_ptr<StonksDb> stonks_db_{};
};
}  // namespace stonks::finance

#endif  // STONKS_SERVICES_V2_FINANCE_DB_CLIENT_SERVER_H_
