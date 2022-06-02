#ifndef STONKS_SERVERS_SERVER_INFO_H_
#define STONKS_SERVERS_SERVER_INFO_H_

#include <memory>

#include "server.h"
#include "stonks_info.h"

namespace stonks {
class InfoServer {
 public:
  static constexpr auto kEndpoint = "/Info";

  explicit InfoServer(int port, std::shared_ptr<Info> entity);

 private:
  [[nodiscard]] auto GetSymbols() -> network::Endpoint;
  [[nodiscard]] auto GetStrategyNames() -> network::Endpoint;
  [[nodiscard]] auto GetPriceTicks() -> network::Endpoint;

  network::Server server_;
  std::shared_ptr<Info> entity_{};
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_INFO_H_
