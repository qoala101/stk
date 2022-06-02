#include "client_finance_db.h"
#include "client_proxy.h"
#include "server_finance_db.h"
#include "stonks_db_updater_symbols_info.h"

auto main(int /*unused*/, const char* /*unused*/[]) -> int {
  const auto proxy = stonks::client::Proxy();
  constexpr auto endpoint = stonks::server::StonksDb::kEndpoint;
  const auto port = proxy.GetEndpointPort(endpoint);

  const auto app = stonks::DbUpdaterSymbolsInfo{
      std::make_shared<stonks::client::FinanceDb>(*port)};
  static_cast<void>(getchar());
}