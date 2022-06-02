#include <string>

#include "client_proxy.h"
#include "finance_db.h"
#include "server_finance_db.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  auto proxy = stonks::ProxyClient{};
  constexpr auto endpoint = stonks::StonksDbServer::kEndpoint;
  proxy.RegisterEndpoint(endpoint);
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app = stonks::StonksDbServer{
      *port,
      std::make_shared<stonks::finance::FinanceDb>("StrategyTestingDb.db")};
  static_cast<void>(getchar());
}