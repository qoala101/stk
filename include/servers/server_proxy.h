#ifndef STONKS_SERVERS_SERVER_PROXY_H_
#define STONKS_SERVERS_SERVER_PROXY_H_

#include "endpoint.h"
#include "proxy.h"
#include "server.h"

namespace stonks {
class ProxyServer {
 public:
  static constexpr auto kPort = 6506;
  static constexpr auto kEndpoint = "/Proxy";

  explicit ProxyServer();

 private:
  [[nodiscard]] auto GetEndpointPort() -> network::Endpoint;
  [[nodiscard]] auto RegisterEndpoint() -> network::Endpoint;

  network::Server server_;
  network::Proxy proxy_;
};
}  // namespace stonks

#endif  // STONKS_SERVERS_SERVER_PROXY_H_
