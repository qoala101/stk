#include "finance_db_client_server.h"

#include "endpoint.h"

namespace stonks::finance {
namespace endpoint {
auto SelectAssets() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectAssets",
      .response_body = json::Type<std::optional<std::vector<std::string>>>{}};
}

auto SelectSymbols() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbols",
      .response_body = json::Type<std::optional<std::vector<Symbol>>>{}};
}

auto SelectSymbolPriceTicks() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbolPriceTicks",
      .params = {{"limit", json::Type<std::optional<int>>{}},
                 {"period", json::Type<std::optional<Period>>{}},
                 {"symbols", json::Type<std::optional<std::vector<Symbol>>>{}}},
      .response_body =
          json::Type<std::optional<std::vector<SymbolPriceTick>>>{}};
}

auto InsertSymbolPriceTick() -> network::EndpointDesc {
  return network::EndpointDesc{.method = web::http::methods::POST,
                               .relative_uri = "/InsertSymbolPriceTick",
                               .request_body = json::Type<SymbolPriceTick>{},
                               .response_body = json::Type<bool>{}};
}
}  // namespace endpoint

FinanceDbClient::FinanceDbClient(std::string_view base_uri)
    : client_{base_uri} {}

auto FinanceDbClient::SelectAssets() const
    -> std::optional<std::vector<std::string>> {
  return std::any_cast<std::optional<std::vector<std::string>>>(
      client_.Execute(endpoint::SelectAssets()));
}

auto FinanceDbClient::SelectSymbols() const
    -> std::optional<std::vector<Symbol>> {
  return std::any_cast<std::optional<std::vector<Symbol>>>(
      client_.Execute(endpoint::SelectSymbols()));
}

auto FinanceDbClient::InsertSymbolPriceTick(
    const SymbolPriceTick &symbol_price_tick) -> bool {
  return std::any_cast<bool>(
      client_.Execute(endpoint::InsertSymbolPriceTick(), symbol_price_tick));
}

auto FinanceDbClient::SelectSymbolPriceTicks(
    std::optional<int> limit, const std::optional<Period> &period,
    const std::optional<std::vector<Symbol>> &symbols) const
    -> std::optional<std::vector<SymbolPriceTick>> {
  return std::any_cast<std::optional<std::vector<SymbolPriceTick>>>(
      client_.Execute(
          endpoint::SelectSymbolPriceTicks(),
          {{"limit", limit}, {"period", period}, {"symbols", symbols}}));
}

FinanceDbServer::FinanceDbServer(std::string_view base_uri,
                                 FinanceDb finance_db)
    : server_{base_uri,
              {network::Endpoint{
                   .desc = endpoint::SelectAssets(),
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return finance_db_.SelectAssets();
                   }}},
               network::Endpoint{
                   .desc = endpoint::SelectSymbols(),
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return finance_db_.SelectSymbols();
                   }}},
               network::Endpoint{
                   .desc = endpoint::InsertSymbolPriceTick(),
                   .handler =
                       std::function<std::any(std::any)>{
                           [this](std::any request_body) -> std::any {
                             return finance_db_.InsertSymbolPriceTick(
                                 std::any_cast<SymbolPriceTick>(
                                     std::move(request_body)));
                           }

                       }},
               network::Endpoint{
                   .desc = endpoint::SelectSymbolPriceTicks(),
                   .handler =
                       std::function<std::any(std::map<std::string, std::any>)>{
                           [this](std::map<std::string, std::any> params)
                               -> std::any {
                             return finance_db_.SelectSymbolPriceTicks(
                                 std::any_cast<std::optional<int>>(
                                     std::move(params.at("limit"))),
                                 std::any_cast<std::optional<Period>>(
                                     std::move(params.at("period"))),
                                 std::any_cast<
                                     std::optional<std::vector<Symbol>>>(
                                     std::move(params.at("symbols"))));
                           }}}}},
      finance_db_{std::move(finance_db)} {}
}  // namespace stonks::finance