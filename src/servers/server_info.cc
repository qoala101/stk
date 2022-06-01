#include "server_info.h"

#include "endpoint.h"
#include "endpoints_info.h"

namespace stonks {
InfoServer::InfoServer(std::string_view base_uri)
    : server_{base_uri, {GetSymbols(), GetStrategyNames(), GetPriceTicks()}} {}

auto InfoServer::GetSymbols() -> network::Endpoint {
  return {InfoEndpoints::GetSymbols(),
          network::HasResult{[this]() { return info_.GetSymbols(); }}};
}

auto InfoServer::GetStrategyNames() -> network::Endpoint {
  return {InfoEndpoints::GetStrategyNames(),
          network::HasResult{[this]() { return info_.GetStrategyNames(); }}};
}

auto InfoServer::GetPriceTicks() -> network::Endpoint {
  return {
      InfoEndpoints::GetPriceTicks(),
      network::HasResultTakesParams{[this](network::Params params) {
        return info_.GetPriceTicks(params.Get<finance::SymbolName>("symbol"));
      }}};
}
}  // namespace stonks