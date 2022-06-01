#include "client_finance_db.h"
#include "client_proxy.h"
#include "server_finance_db.h"
#include "stonks_db_updater_price_ticks.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto proxy = stonks::ProxyClient();
  constexpr auto endpoint = stonks::StonksDbServer::kEndpoint;
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app =
      stonks::DbUpdaterPriceTicks{std::make_shared<stonks::FinanceDbClient>(
          "http://localhost:" + std::to_string(*port) + endpoint)};
  static_cast<void>(getchar());
}