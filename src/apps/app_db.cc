#include <stdio.h>

#include <memory>
#include <optional>

#include "client_proxy.h"
#include "finance_db.h"
#include "server_finance_db.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  auto proxy = stonks::client::Proxy{};
  constexpr auto endpoint = stonks::server::StonksDb::kEndpoint;
  proxy.RegisterEndpoint(endpoint);
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app = stonks::server::StonksDb{
      *port,
      std::make_shared<stonks::finance::FinanceDb>("StrategyTestingDb.db")};
  static_cast<void>(getchar());
}