#include "server_info.h"

#include <gsl/assert>
#include <string_view>
#include <type_traits>
#include <utility>

#include "endpoints_info.h"
#include "finance_types.h"
#include "uri.h"

namespace stonks::server {
Info::Info(int port, std::shared_ptr<stonks::Info> entity)
    : server_{network::LocalUri{port, kEndpoint},
              {GetSymbols(), GetStrategyNames(), GetPriceTicks()}},
      entity_{std::move(entity)} {
  Expects(entity_ != nullptr);
}

auto Info::GetSymbols() -> network::Endpoint {
  return {endpoints::Info::GetSymbols(),
          network::HasResult{[this]() { return entity_->GetSymbols(); }}};
}

auto Info::GetStrategyNames() -> network::Endpoint {
  return {endpoints::Info::GetStrategyNames(),
          network::HasResult{[this]() { return entity_->GetStrategyNames(); }}};
}

auto Info::GetPriceTicks() -> network::Endpoint {
  return {endpoints::Info::GetPriceTicks(),
          network::HasResultTakesParams{[this](network::Params params) {
            return entity_->GetPriceTicks(
                std::move(params).Take<finance::SymbolName>("symbol"));
          }}};
}
}  // namespace stonks::server