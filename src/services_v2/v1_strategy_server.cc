#include "v1_strategy_server.h"

#include "finance_types.h"

namespace stonks {
V1StrategyServer::V1StrategyServer(std::string_view base_uri)
    : server_{
          base_uri,
          {network::Endpoint{
              .desc =
                  network::EndpointDesc{
                      .method = web::http::methods::GET,
                      .relative_uri = "/Run",
                      .params = {{"symbol", json::Type<finance::SymbolName>{}}},
                      .response_body = json::Type<
                          std::vector<finance::SymbolBalanceTick>>{}},
              .handler =
                  std::function<std::any(std::map<std::string, std::any>)>{
                      [this](
                          std::map<std::string, std::any> params) -> std::any {
                        return strategy_.Run(std::any_cast<finance::SymbolName>(
                            params.at("symbol")));
                      }}}}} {}
}  // namespace stonks