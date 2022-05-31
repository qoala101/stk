#include "info_server.h"

namespace stonks {
InfoServer::InfoServer(std::string_view base_uri)
    : server_{base_uri,
              {network::Endpoint{
                   .desc =
                       network::EndpointDesc{
                           .method = web::http::methods::GET,
                           .relative_uri = "/GetSymbols",
                           .response_body =
                               json::Type<std::vector<finance::SymbolName>>{}},
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return info_.GetSymbols();
                   }}},
               network::Endpoint{
                   .desc =
                       network::EndpointDesc{
                           .method = web::http::methods::GET,
                           .relative_uri = "/GetStrategyNames",
                           .response_body =
                               json::Type<std::vector<std::string>>{}},
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return info_.GetStrategyNames();
                   }}},
               network::Endpoint{
                   .desc =
                       network::EndpointDesc{
                           .method = web::http::methods::GET,
                           .relative_uri = "/GetPriceTicks",
                           .params = {{"symbol",
                                       json::Type<finance::SymbolName>{}}},
                           .response_body = json::Type<
                               std::vector<finance::SymbolPriceTick>>{}},
                   .handler =
                       std::function<std::any(std::map<std::string, std::any>)>{
                           [this](std::map<std::string, std::any> params)
                               -> std::any {
                             return info_.GetPriceTicks(
                                 std::any_cast<finance::SymbolName>(
                                     params.at("symbol")));
                           }}}}} {}
}  // namespace stonks