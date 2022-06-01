#include "proxy_client_server.h"

namespace stonks {
namespace endpoint {
auto GetEndpointPort() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::GET,
      .relative_uri = "/GetEndpointPort",
      .params = {{"endpoint", json::Type<std::string>{}}},
      .response_body = json::Type<std::optional<int>>{}};
}

auto RegisterEndpoint() -> network::EndpointDesc {
  return network::EndpointDesc{
      .method = web::http::methods::POST,
      .relative_uri = "/RegisterEndpoint",
      .params = {{"endpoint", json::Type<std::string>{}}}};
}
}  // namespace endpoint

ProxyClient::ProxyClient()
    : client_{"http://localhost:" + std::to_string(ProxyServer::kPort) +
              ProxyServer::kEndpoint} {}

auto ProxyClient::GetEndpointPort(std::string_view endpoint) const
    -> std::optional<int> {
  return std::any_cast<std::optional<int>>(
      client_.Execute(endpoint::GetEndpointPort(), {{"endpoint", endpoint}}));
}

void ProxyClient::RegisterEndpoint(std::string_view endpoint) {
  client_.Execute(endpoint::RegisterEndpoint(), {{"endpoint", endpoint}});
}

ProxyServer::ProxyServer()
    : server_{
          "http://localhost:" + std::to_string(kPort) + kEndpoint,
          {network::Endpoint{
               .desc = endpoint::GetEndpointPort(),
               .handler =
                   std::function<std::any(std::map<std::string, std::any>)>{
                       [this](
                           std::map<std::string, std::any> params) -> std::any {
                         return proxy_.GetEndpointPort(
                             std::any_cast<std::string>(params.at("endpoint")));
                       }}},
           network::Endpoint{
               .desc = endpoint::RegisterEndpoint(),
               .handler = std::function<void(std::map<std::string, std::any>)>{
                   [this](std::map<std::string, std::any> params) {
                     proxy_.RegisterEndpoint(
                         std::any_cast<std::string>(params.at("endpoint")));
                   }}}}} {}
}  // namespace stonks