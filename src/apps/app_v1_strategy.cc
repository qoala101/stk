#include "proxy_client_server.h"
#include "v1_strategy_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  auto proxy = stonks::ProxyClient{};
  constexpr auto endpoint = stonks::V1StrategyServer::kEndpoint;
  proxy.RegisterEndpoint(endpoint);
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app = stonks::V1StrategyServer{
      "http://localhost:" + std::to_string(*port) + endpoint};
  static_cast<void>(getchar());
}