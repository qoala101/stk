#ifndef STONKS_CLIENTS_CLIENT_PROXY_H_
#define STONKS_CLIENTS_CLIENT_PROXY_H_

#include <optional>
#include <string_view>

#include "client.h"
#include "proxy.h"

namespace stonks::client {
class Proxy : public network::ProxyInterface {
 public:
  explicit Proxy();

  [[nodiscard]] auto GetEndpointPort(std::string_view endpoint) const
      -> std::optional<int> override;
  void RegisterEndpoint(std::string_view endpoint) override;

 private:
  network::Client client_;
};
}  // namespace stonks::client

#endif  // STONKS_CLIENTS_CLIENT_PROXY_H_
