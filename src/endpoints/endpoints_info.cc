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
          .relative_uri = "/GetSymbols"};
}

auto endpoints::Info::GetStrategyNames() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetStrategyNames"};
}

auto endpoints::Info::GetPriceTicks() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/GetPriceTicks"};
}
}  // namespace stonks