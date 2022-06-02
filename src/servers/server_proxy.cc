#include "server_proxy.h"

#include <gsl/assert>

#include "endpoints_proxy.h"

namespace stonks {
ProxyServer::ProxyServer(std::shared_ptr<network::Proxy> entity)
    : server_{kPort, kEndpoint, {GetEndpointPort(), RegisterEndpoint()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto ProxyServer::GetEndpointPort() -> network::Endpoint {
  return {
      ProxyEndpoints::GetEndpointPort(),
      network::HasResultTakesParams{[this](network::Params params) {
        return entity_->GetEndpointPort(params.Get<std::string>("endpoint"));
      }}};
}

auto ProxyServer::RegisterEndpoint() -> network::Endpoint {
  return {ProxyEndpoints::RegisterEndpoint(),
          network::NoResultTakesParams{[this](network::Params params) {
            entity_->RegisterEndpoint(params.Get<std::string>("endpoint"));
          }}};
}
}  // namespace stonks