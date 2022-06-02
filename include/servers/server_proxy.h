#ifndef STONKS_SERVERS_SERVER_PROXY_H_
#define STONKS_SERVERS_SERVER_PROXY_H_

#include <memory>

#include "proxy.h"
#include "server.h"

namespace stonks {
class ProxyServer {
 public:
  static constexpr auto kPort = 6506;
  static constexpr auto kEndpoint = "/Proxy";

  explicit ProxyServer(std::shared_ptr<network::Proxy> entity);

 private:
  [[nodiscard]] auto GetEndpointPort() -> network::Endpoint;
  [[nodiscard]] auto RegisterEndpoint() -> network::Endpoint;

  network::Server server_;
  std::shared_ptr<network::Proxy> entity_{};
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_PROXY_H_
