#include "server_v1_strategy.h"

#include "endpoint.h"
#include "endpoints_v1_strategy.h"

namespace stonks {
V1StrategyServer::V1StrategyServer(std::string_view base_uri)
    : server_{base_uri, {Run()}} {}

auto V1StrategyServer::Run() -> network::Endpoint {
  return {V1StrategyEndpoints::Run(),
          network::HasResultTakesParams{[this](network::Params params) {
            return strategy_.Run(params.Get<finance::SymbolName>("symbol"));
          }}};
}
}  // namespace stonks