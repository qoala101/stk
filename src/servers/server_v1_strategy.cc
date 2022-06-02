#include "server_v1_strategy.h"

#include <gsl/assert>

#include "endpoints_v1_strategy.h"

namespace stonks {
V1StrategyServer::V1StrategyServer(int port, std::shared_ptr<V1Strategy> entity)
    : server_{network::LocalUri{port, kEndpoint}, {Run()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto V1StrategyServer::Run() -> network::Endpoint {
  return {V1StrategyEndpoints::Run(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->Run(params.Get<finance::SymbolName>("symbol"));
          }}};
}
}  // namespace stonks