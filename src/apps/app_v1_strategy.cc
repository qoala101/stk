#include "client_proxy.h"
#include "server_v1_strategy.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  auto proxy = stonks::ProxyClient{};
  constexpr auto endpoint = stonks::V1StrategyServer::kEndpoint;
  proxy.RegisterEndpoint(endpoint);
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app =
      stonks::V1StrategyServer{*port, std::make_shared<stonks::V1Strategy>()};
  static_cast<void>(getchar());
}