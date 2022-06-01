#include "endpoints_v1_strategy.h"

namespace stonks {
auto V1StrategyEndpoints::Run() -> network::EndpointDesc {
  return {
      .method = web::http::methods::GET,
      .relative_uri = "/Run",
      .params = {{"symbol", json::Type<finance::SymbolName>{}}},
      .response_body = json::Type<std::vector<finance::SymbolBalanceTick>>{}};
}
}  // namespace stonks