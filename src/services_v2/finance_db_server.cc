#include "finance_db_server.h"

#include "client_server_types.h"
#include "finance_db.h"
#include "finance_db_endpoints.h"
#include "finance_types.h"
#include "server.h"

namespace stonks::finance {
FinanceDbServer::FinanceDbServer(std::string_view base_uri,
                                 std::string_view db_uri)
    : network::Server{base_uri,
                      {network::Endpoint{.desc = SelectAssetsEndpoint(),
                                         .handler = SelectAssetsHandler()},
                       network::Endpoint{.desc = SelectSymbolsEndpoint(),
                                         .handler = SelectSymbolsHandler()},
                       network::Endpoint{
                           .desc = InsertSymbolPriceTickEndpoint(),
                           .handler = InsertSymbolPriceTickHandler()},
                       network::Endpoint{
                           .desc = SelectSymbolPriceTicksEndpoint(),
                           .handler = SelectSymbolPriceTicksHandler()}}},
      finance_db_{db_uri} {}

auto FinanceDbServer::SelectAssetsHandler()
    -> stonks::network::EndpointHandler {
  return
      // TODO(vh): make all those params as single struct
      [this](const std::map<std::string, stonks::json::Any> &,
             const stonks::json::Any &) { return finance_db_.SelectAssets(); };
}

auto FinanceDbServer::SelectSymbolsHandler()
    -> stonks::network::EndpointHandler {
  return
      [this](const std::map<std::string, stonks::json::Any> &,
             const stonks::json::Any &) { return finance_db_.SelectSymbols(); };
}

auto FinanceDbServer::InsertSymbolPriceTickHandler()
    -> stonks::network::EndpointHandler {
  return [this](const std::map<std::string, stonks::json::Any> &,
                stonks::json::Any request_body) {
    return finance_db_.InsertSymbolPriceTick(
        std::any_cast<SymbolPriceTick>(std::move(request_body.value)));
  };
}

auto FinanceDbServer::SelectSymbolPriceTicksHandler()
    -> stonks::network::EndpointHandler {
  return [this](std::map<std::string, stonks::json::Any> params,
                const stonks::json::Any &) {
    return finance_db_.SelectSymbolPriceTicks(
        // TODO(vh): map.at here would throw if param is missing since it's
        // optional SHOULD PUT NULLOPT PARAM IN THE MAP?
        std::any_cast<std::optional<int>>(std::move(params.at("limit").value)),
        std::any_cast<std::optional<Period>>(
            std::move(params.at("period").value)),
        std::any_cast<std::optional<std::vector<Symbol>>>(
            std::move(params.at("symbols").value)));
  };
}
}  // namespace stonks::finance

// auto main(int /*unused*/, const char * /*unused*/[]) -> int {
//   const auto server = stonks::finance::FinanceDbServer{
//       "http://localhost:6506/FinanceDb", "ten_million_price_ticks.db"};
//   static_cast<void>(getchar());
// }