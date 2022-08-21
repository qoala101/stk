#ifndef STONKS_SERVERS_SERVER_FINANCE_DB_H_
#define STONKS_SERVERS_SERVER_FINANCE_DB_H_

#include "ccutils_not_null.h"

#include "endpoint.h"
#include "server.h"
#include "stonks_db.h"

namespace stonks::server {
class StonksDb {
 public:
  static constexpr auto kEndpoint = "/Db";

  explicit StonksDb(int port, ccutils::Sp<stonks::StonksDb> entity);

 private:
  [[nodiscard]] auto SelectAssets() const -> network::Endpoint;
  [[nodiscard]] auto UpdateAssets() const -> network::Endpoint;
  [[nodiscard]] auto SelectSymbols() const -> network::Endpoint;
  [[nodiscard]] auto SelectSymbolsInfo() const -> network::Endpoint;
  [[nodiscard]] auto UpdateSymbolsInfo() const -> network::Endpoint;
  [[nodiscard]] auto SelectSymbolPriceTicks() const -> network::Endpoint;
  [[nodiscard]] auto InsertSymbolPriceTick() const -> network::Endpoint;

  network::Server server_;
  ccutils::Sp<stonks::StonksDb> entity_{};
};
}  // namespace stonks::server

#endif  // STONKS_SERVERS_SERVER_FINANCE_DB_H_
