#include "server_info.h"

#include <gsl/assert>

#include "endpoints_info.h"

namespace stonks {
InfoServer::InfoServer(int port, std::shared_ptr<Info> entity)
    : server_{network::LocalUri{port, kEndpoint},
              {GetSymbols(), GetStrategyNames(), GetPriceTicks()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto InfoServer::GetSymbols() -> network::Endpoint {
  return {InfoEndpoints::GetSymbols(),
          network::HasResult{[this]() { return entity_->GetSymbols(); }}};
}

auto InfoServer::GetStrategyNames() -> network::Endpoint {
  return {InfoEndpoints::GetStrategyNames(),
          network::HasResult{[this]() { return entity_->GetStrategyNames(); }}};
}

auto InfoServer::GetPriceTicks() -> network::Endpoint {
  return {InfoEndpoints::GetPriceTicks(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->GetPriceTicks(
                params.Get<finance::SymbolName>("symbol"));
          }}};
}
}  // namespace stonks