#include "endpoints_finance_db.h"

#include <cpprest/http_msg.h>

#include <optional>
#include <string>
#include <vector>

#include "finance_types.h"
#include "type.h"
#include "type_variant.h"

namespace stonks {
auto endpoints::FinanceDb::SelectAssets() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectAssets"};
}

auto endpoints::FinanceDb::UpdateAssets() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/UpdateAssets"};
}

auto endpoints::FinanceDb::SelectSymbols() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectSymbols"};
}

auto endpoints::FinanceDb::SelectSymbolsInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectSymbolsInfo"};
}

auto endpoints::FinanceDb::UpdateSymbolsInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/UpdateSymbolsInfo"};
}

auto endpoints::FinanceDb::SelectSymbolPriceTicks() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectSymbolPriceTicks"};
}

auto endpoints::FinanceDb::InsertSymbolPriceTick() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/InsertSymbolPriceTick"};
}
}  // namespace stonks