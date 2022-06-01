#include <string>

#include "finance_db.h"
#include "finance_db_client_server.h"
#include "proxy_client_server.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  auto proxy = stonks::ProxyClient{};
  constexpr auto endpoint = stonks::finance::StonksDbServer::kEndpoint;
  proxy.RegisterEndpoint(endpoint);
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app = stonks::finance::StonksDbServer{
      "http://localhost:" + std::to_string(*port) + endpoint,
      std::make_shared<stonks::finance::FinanceDb>("StrategyTestingDb.db")};
  static_cast<void>(getchar());
}