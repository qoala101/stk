#include "finance_db_client_server.h"

#include <gsl/assert>

#include "endpoint.h"
#include "finance_types.h"

namespace stonks::finance {
namespace endpoint {
auto SelectAssets() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectAssets",
      .response_body = json::Type<std::vector<std::string>>{}};
}

auto UpdateAssets() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::POST,
      .relative_uri = "/UpdateAssets",
      .request_body = json::Type<std::vector<std::string>>{}};
}

auto SelectSymbols() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbols",
      .response_body = json::Type<std::vector<SymbolName>>{}};
}

auto SelectSymbolsInfo() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbolsInfo",
      .response_body = json::Type<std::vector<SymbolInfo>>{}};
}

auto UpdateSymbolsInfo() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::POST,
      .relative_uri = "/UpdateSymbolsInfo",
      .request_body = json::Type<std::vector<SymbolInfo>>{}};
}

auto SelectSymbolPriceTicks() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/SelectSymbolPriceTicks",
      .params = {{"limit", json::Type<std::optional<int>>{}},
                 {"period", json::Type<std::optional<Period>>{}},
                 {"symbols",
                  json::Type<std::optional<std::vector<SymbolName>>>{}}},
      .response_body = json::Type<std::vector<SymbolPriceTick>>{}};
}

auto InsertSymbolPriceTick() -> network::EndpointDesc {
  return network::EndpointDesc{.method = web::http::methods::POST,
                               .relative_uri = "/InsertSymbolPriceTick",
                               .request_body = json::Type<SymbolPriceTick>{}};
}
}  // namespace endpoint

FinanceDbClient::FinanceDbClient(std::string_view base_uri)
    : client_{base_uri} {}

auto FinanceDbClient::SelectAssets() const -> std::vector<std::string> {
  return std::any_cast<std::vector<std::string>>(
      client_.Execute(endpoint::SelectAssets()));
}

void FinanceDbClient::UpdateAssets(std::vector<std::string> assets) {
  client_.Execute(endpoint::UpdateAssets(), std::move(assets));
}

auto FinanceDbClient::SelectSymbols() const -> std::vector<SymbolName> {
  return std::any_cast<std::vector<SymbolName>>(
      client_.Execute(endpoint::SelectSymbols()));
}

auto FinanceDbClient::SelectSymbolsInfo() const -> std::vector<SymbolInfo> {
  return std::any_cast<std::vector<SymbolInfo>>(
      client_.Execute(endpoint::SelectSymbolsInfo()));
}

void FinanceDbClient::UpdateSymbolsInfo(std::vector<SymbolInfo> symbols_info) {
  client_.Execute(endpoint::UpdateSymbolsInfo(), std::move(symbols_info));
}

auto FinanceDbClient::SelectSymbolPriceTicks(
    std::optional<int> limit, const std::optional<Period> &period,
    const std::optional<std::vector<SymbolName>> &symbols) const
    -> std::vector<SymbolPriceTick> {
  return std::any_cast<std::vector<SymbolPriceTick>>(client_.Execute(
      endpoint::SelectSymbolPriceTicks(),
      {{"limit", limit}, {"period", period}, {"symbols", symbols}}));
}

void FinanceDbClient::InsertSymbolPriceTick(
    const SymbolPriceTick &symbol_price_tick) {
  client_.Execute(endpoint::InsertSymbolPriceTick(), symbol_price_tick);
}

StonksDbServer::StonksDbServer(std::string_view base_uri,
                               std::shared_ptr<StonksDb> stonks_db)
    : server_{base_uri,
              {network::Endpoint{
                   .desc = endpoint::SelectAssets(),
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return stonks_db_->SelectAssets();
                   }}},
               network::Endpoint{
                   .desc = endpoint::UpdateAssets(),
                   .handler =
                       std::function<void(std::any)>{
                           [this](std::any request_body) {
                             stonks_db_->UpdateAssets(
                                 std::any_cast<std::vector<std::string>>(
                                     std::move(request_body)));
                           }}},
               network::Endpoint{
                   .desc = endpoint::SelectSymbols(),
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return stonks_db_->SelectSymbols();
                   }}},
               network::Endpoint{
                   .desc = endpoint::SelectSymbolsInfo(),
                   .handler = std::function<std::any()>{[this]() -> std::any {
                     return stonks_db_->SelectSymbolsInfo();
                   }}},
               network::Endpoint{
                   .desc = endpoint::UpdateSymbolsInfo(),
                   .handler =
                       std::function<void(std::any)>{
                           [this](std::any request_body) {
                             stonks_db_->UpdateSymbolsInfo(
                                 std::any_cast<std::vector<SymbolInfo>>(
                                     std::move(request_body)));
                           }}},
               network::Endpoint{
                   .desc = endpoint::SelectSymbolPriceTicks(),
                   .handler =
                       std::function<std::any(std::map<std::string, std::any>)>{
                           [this](std::map<std::string, std::any> params)
                               -> std::any {
                             return stonks_db_->SelectSymbolPriceTicks(
                                 std::any_cast<std::optional<int>>(
                                     std::move(params.at("limit"))),
                                 std::any_cast<std::optional<Period>>(
                                     std::move(params.at("period"))),
                                 std::any_cast<
                                     std::optional<std::vector<SymbolName>>>(
                                     std::move(params.at("symbols"))));
                           }}},
               network::Endpoint{.desc = endpoint::InsertSymbolPriceTick(),
                                 .handler =
                                     std::function<void(std::any)>{
                                         [this](std::any request_body) {
                                           stonks_db_->InsertSymbolPriceTick(
                                               std::any_cast<SymbolPriceTick>(
                                                   std::move(request_body)));
                                         }

                                     }}}},
      stonks_db_{std::move(stonks_db)} {
  Expects(stonks_db_ != nullptr);
}
}  // namespace stonks::finance