#include "stonks_info.h"

#include "proxy_client_server.h"

namespace stonks {
namespace {
[[nodiscard]] auto GetDbEndpoint() -> std::string {
  const auto proxy = stonks::ProxyClient();
  constexpr auto endpoint = stonks::finance::StonksDbServer::kEndpoint;
  const auto port = proxy.GetEndpointPort(endpoint);
  return "http://localhost:" + std::to_string(*port) + endpoint;
}
}  // namespace

Info::Info() : finance_db_{GetDbEndpoint()} {}

[[nodiscard]] auto Info::GetSymbols() const
    -> std::vector<finance::SymbolName> {
  return finance_db_.SelectSymbols();
}

[[nodiscard]] auto Info::GetStrategyNames() const -> std::vector<std::string> {
  return {"V1Strategy"};
}

[[nodiscard]] auto Info::GetPriceTicks(const finance::SymbolName &symbol) const
    -> std::vector<finance::SymbolPriceTick> {
  return finance_db_.SelectSymbolPriceTicks(std::nullopt, std::nullopt,
                                            {{symbol}});
}
}  // namespace stonks