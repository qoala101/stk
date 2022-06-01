#include "server_proxy.h"

#include "endpoint.h"
#include "endpoints_proxy.h"

namespace stonks {
ProxyServer::ProxyServer()
    : server_{"http://localhost:" + std::to_string(kPort) + kEndpoint,
              {GetEndpointPort(), RegisterEndpoint()}} {}

auto ProxyServer::GetEndpointPort() -> network::Endpoint {
  return {ProxyEndpoints::GetEndpointPort(),
          network::HasResultTakesParams{[this](network::Params params) {
            return proxy_.GetEndpointPort(params.Get<std::string>("endpoint"));
          }}};
}

auto ProxyServer::RegisterEndpoint() -> network::Endpoint {
  return {ProxyEndpoints::RegisterEndpoint(),
          network::NoResultTakesParams{[this](network::Params params) {
            proxy_.RegisterEndpoint(params.Get<std::string>("endpoint"));
          }}};
}
}  // namespace stonks