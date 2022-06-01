#ifndef STONKS_SERVICES_V2_PROXY_CLIENT_SERVER_H_
#define STONKS_SERVICES_V2_PROXY_CLIENT_SERVER_H_

#include <string_view>

#include "client.h"
#include "proxy.h"
#include "server.h"

namespace stonks {
// TODO(vh): create a separate file with all clients and servers similar to json
// conversions Client<Proxy, ProxyInterface> Server<Proxy, ProxyInterface> or
// dont use interface, and use some static check instead
// TODO(vh): create generic App?
class ProxyClient : public network::ProxyInterface {
 public:
  explicit ProxyClient();

  /**
   * @copydoc ProxyInterface::GetEndpointPort
   */
  [[nodiscard]] auto GetEndpointPort(std::string_view endpoint) const
      -> std::optional<int> override;

  /**
   * @copydoc ProxyInterface::RegisterEndpoint
   */
  void RegisterEndpoint(std::string_view endpoint) override;

 private:
  network::Client client_;
};

class ProxyServer {
 public:
  static constexpr auto kPort = 6506;
  static constexpr auto kEndpoint = "/Proxy";

  explicit ProxyServer();

 private:
  network::Server server_;
  network::Proxy proxy_;
};
}  // namespace stonks

#endif  // STONKS_SERVICES_V2_PROXY_CLIENT_SERVER_H_
