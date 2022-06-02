#include "endpoints_finance_db.h"

namespace stonks {
auto endpoints::FinanceDb::SelectAssets() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectAssets",
          .response_body = json::Type<std::vector<std::string>>{}};
}

auto endpoints::FinanceDb::UpdateAssets() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/UpdateAssets",
          .request_body = json::Type<std::vector<std::string>>{}};
}

auto endpoints::FinanceDb::SelectSymbols() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectSymbols",
          .response_body = json::Type<std::vector<finance::SymbolName>>{}};
}

auto endpoints::FinanceDb::SelectSymbolsInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectSymbolsInfo",
          .response_body = json::Type<std::vector<finance::SymbolInfo>>{}};
}

auto endpoints::FinanceDb::UpdateSymbolsInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/UpdateSymbolsInfo",
          .request_body = json::Type<std::vector<finance::SymbolInfo>>{}};
}

auto endpoints::FinanceDb::SelectSymbolPriceTicks() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/SelectSymbolPriceTicks",
          .params =
              {{"limit", json::Type<std::optional<int>>{}},
               {"period", json::Type<std::optional<finance::Period>>{}},
               {"symbols",
                json::Type<std::optional<std::vector<finance::SymbolName>>>{}}},
          .response_body = json::Type<std::vector<finance::SymbolPriceTick>>{}};
}

auto endpoints::FinanceDb::InsertSymbolPriceTick() -> network::EndpointDesc {
  return {.method = web::http::methods::POST,
          .relative_uri = "/InsertSymbolPriceTick",
          .request_body = json::Type<finance::SymbolPriceTick>{}};
}
}  // namespace stonks