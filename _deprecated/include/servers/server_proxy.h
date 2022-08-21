#ifndef STONKS_SERVERS_SERVER_PROXY_H_
#define STONKS_SERVERS_SERVER_PROXY_H_

#include "ccutils_not_null.h"

#include "endpoint.h"
#include "proxy.h"
#include "server.h"

namespace stonks::server {
class Proxy {
 public:
  static constexpr auto kPort = 6506;
  static constexpr auto kEndpoint = "/Proxy";

  explicit Proxy(ccutils::Sp<network::Proxy> entity);

 private:
  [[nodiscard]] auto GetEndpointPort() -> network::Endpoint;
  [[nodiscard]] auto RegisterEndpoint() -> network::Endpoint;

  network::Server server_;
  ccutils::Sp<network::Proxy> entity_{};
};
}  // namespace stonks::server

#endif  // STONKS_SERVERS_SERVER_PROXY_H_
