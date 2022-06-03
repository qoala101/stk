#include "endpoints_info.h"

#include <cpprest/http_msg.h>

#include <optional>
#include <string>
#include <vector>

#include "finance_types.h"
#include "type.h"
#include "type_variant.h"

namespace stonks {
auto endpoints::Info::GetSymbols() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetSymbols",
          .response_body = json::Type<std::vector<finance::SymbolName>>{}};
}

auto endpoints::Info::GetStrategyNames() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetStrategyNames",
          .response_body = json::Type<std::vector<std::string>>{}};
}

auto endpoints::Info::GetPriceTicks() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetPriceTicks",
          .params = {{"symbol", json::Type<finance::SymbolName>{}}},
          .response_body = json::Type<std::vector<finance::SymbolPriceTick>>{}};
}
}  // namespace stonks