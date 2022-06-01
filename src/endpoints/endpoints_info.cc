#include "endpoints_info.h"

namespace stonks {
auto InfoEndpoints::GetSymbols() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetSymbols",
          .response_body = json::Type<std::vector<finance::SymbolName>>{}};
}

auto InfoEndpoints::GetStrategyNames() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetStrategyNames",
          .response_body = json::Type<std::vector<std::string>>{}};
}

auto InfoEndpoints::GetPriceTicks() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetPriceTicks",
          .params = {{"symbol", json::Type<finance::SymbolName>{}}},
          .response_body = json::Type<std::vector<finance::SymbolPriceTick>>{}};
}
}  // namespace stonks