#include "stonks_info.h"

#include <optional>

#include "client_proxy.h"
#include "server_finance_db.h"

namespace stonks {
namespace {
[[nodiscard]] auto GetDbPort() -> int {
  const auto proxy = stonks::client::Proxy();
  constexpr auto endpoint = stonks::server::StonksDb::kEndpoint;
  const auto port = proxy.GetEndpointPort(endpoint);
  return *port;
}
}  // namespace

Info::Info() : finance_db_{GetDbPort()} {}

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