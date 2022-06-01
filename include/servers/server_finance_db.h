#ifndef STONKS_SERVERS_SERVER_FINANCE_DB_H_
#define STONKS_SERVERS_SERVER_FINANCE_DB_H_

#include <memory>
#include <string_view>

#include "endpoint.h"
#include "server.h"
#include "stonks_db.h"

namespace stonks {
class StonksDbServer {
 public:
  static constexpr auto kEndpoint = "/Db";

  explicit StonksDbServer(std::string_view base_uri,
                          std::shared_ptr<StonksDb> stonks_db);

 private:
  [[nodiscard]] auto SelectAssets() const -> network::Endpoint;
  [[nodiscard]] auto UpdateAssets() const -> network::Endpoint;
  [[nodiscard]] auto SelectSymbols() const -> network::Endpoint;
  [[nodiscard]] auto SelectSymbolsInfo() const -> network::Endpoint;
  [[nodiscard]] auto UpdateSymbolsInfo() const -> network::Endpoint;
  [[nodiscard]] auto SelectSymbolPriceTicks() const -> network::Endpoint;
  [[nodiscard]] auto InsertSymbolPriceTick() const -> network::Endpoint;

  network::Server server_;
  std::shared_ptr<StonksDb> stonks_db_{};
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_FINANCE_DB_H_
