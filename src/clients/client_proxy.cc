#include "client_proxy.h"

#include "endpoints_proxy.h"
#include "server_proxy.h"

namespace stonks::client {
Proxy::Proxy()
    : client_{
          network::LocalUri{server::Proxy::kPort, server::Proxy::kEndpoint}} {}

auto Proxy::GetEndpointPort(std::string_view endpoint) const
    -> std::optional<int> {
  return client_
      .Execute(endpoints::Proxy::GetEndpointPort(), {{"endpoint", endpoint}})
      .Get<std::optional<int>>();
}

void Proxy::RegisterEndpoint(std::string_view endpoint) {
  client_.Execute(endpoints::Proxy::RegisterEndpoint(),
                  {{"endpoint", endpoint}});
}
}  // namespace stonks::client