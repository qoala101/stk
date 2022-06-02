#include "client_proxy.h"

#include "endpoints_proxy.h"
#include "server_proxy.h"

namespace stonks {
ProxyClient::ProxyClient()
    : client_{network::LocalUri{ProxyServer::kPort, ProxyServer::kEndpoint}} {}

auto ProxyClient::GetEndpointPort(std::string_view endpoint) const
    -> std::optional<int> {
  return client_
      .Execute(ProxyEndpoints::GetEndpointPort(), {{"endpoint", endpoint}})
      .Get<std::optional<int>>();
}

void ProxyClient::RegisterEndpoint(std::string_view endpoint) {
  client_.Execute(ProxyEndpoints::RegisterEndpoint(), {{"endpoint", endpoint}});
}
}  // namespace stonks