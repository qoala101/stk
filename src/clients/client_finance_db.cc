#include "client_finance_db.h"

#include "endpoints_finance_db.h"
#include "server_finance_db.h"

namespace stonks {
FinanceDbClient::FinanceDbClient(int port)
    : client_{network::LocalUri{port, StonksDbServer::kEndpoint}} {}

auto FinanceDbClient::SelectAssets() const -> std::vector<std::string> {
  return client_.Execute(FinanceDbEndpoints::SelectAssets())
      .Get<std::vector<std::string>>();
}

void FinanceDbClient::UpdateAssets(std::vector<std::string> assets) {
  client_.Execute(FinanceDbEndpoints::UpdateAssets(), std::move(assets));
}

auto FinanceDbClient::SelectSymbols() const
    -> std::vector<finance::SymbolName> {
  return client_.Execute(FinanceDbEndpoints::SelectSymbols())
      .Get<std::vector<finance::SymbolName>>();
}

auto FinanceDbClient::SelectSymbolsInfo() const
    -> std::vector<finance::SymbolInfo> {
  return client_.Execute(FinanceDbEndpoints::SelectSymbolsInfo())
      .Get<std::vector<finance::SymbolInfo>>();
}

void FinanceDbClient::UpdateSymbolsInfo(
    std::vector<finance::SymbolInfo> symbols_info) {
  client_.Execute(FinanceDbEndpoints::UpdateSymbolsInfo(),
                  std::move(symbols_info));
}

auto FinanceDbClient::SelectSymbolPriceTicks(
    std::optional<int> limit, const std::optional<finance::Period> &period,
    const std::optional<std::vector<finance::SymbolName>> &symbols) const
    -> std::vector<finance::SymbolPriceTick> {
  return client_
      .Execute(FinanceDbEndpoints::SelectSymbolPriceTicks(),
               {{"limit", limit}, {"period", period}, {"symbols", symbols}})
      .Get<std::vector<finance::SymbolPriceTick>>();
}

void FinanceDbClient::InsertSymbolPriceTick(
    const finance::SymbolPriceTick &symbol_price_tick) {
  client_.Execute(FinanceDbEndpoints::InsertSymbolPriceTick(),
                  symbol_price_tick);
}
}  // namespace stonks