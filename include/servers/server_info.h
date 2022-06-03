#ifndef STONKS_SERVERS_SERVER_INFO_H_
#define STONKS_SERVERS_SERVER_INFO_H_

#include <memory>

#include "endpoint.h"
#include "server.h"
#include "stonks_info.h"

namespace stonks::server {
class Info {
 public:
  static constexpr auto kEndpoint = "/Info";

  explicit Info(int port, std::shared_ptr<stonks::Info> entity);

 private:
  [[nodiscard]] auto GetSymbols() -> network::Endpoint;
  [[nodiscard]] auto GetStrategyNames() -> network::Endpoint;
  [[nodiscard]] auto GetPriceTicks() -> network::Endpoint;

  network::Server server_;
  std::shared_ptr<stonks::Info> entity_{};
};
}  // namespace stonks::server

#endif  // STONKS_SERVERS_SERVER_INFO_H_
