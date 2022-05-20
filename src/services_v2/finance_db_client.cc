#include "finance_db_client.h"

#include "any_types.h"
#include "finance_db_endpoints.h"

namespace stonks::finance {
FinanceDbClient::FinanceDbClient(std::string_view base_uri)
    : network::Client{base_uri} {}

auto FinanceDbClient::SelectAssets() const
    -> std::optional<std::vector<std::string>> {
  return std::any_cast<std::optional<std::vector<std::string>>>(
      Execute(SelectAssetsEndpoint()));
}

auto FinanceDbClient::SelectSymbols() const
    -> std::optional<std::vector<Symbol>> {
  return std::any_cast<std::optional<std::vector<Symbol>>>(
      Execute(SelectSymbolsEndpoint()));
}

auto FinanceDbClient::InsertSymbolPriceTick(
    const SymbolPriceTick &symbol_price_tick) -> bool {
  return std::any_cast<bool>(
      Execute(InsertSymbolPriceTickEndpoint(), {},
              json::Any{.value = symbol_price_tick,
                        .converter = json::JsonConverter<SymbolPriceTick>{}}));
}

auto FinanceDbClient::SelectSymbolPriceTicks(
    std::optional<int> limit, const std::optional<Period> &period,
    const std::optional<std::vector<Symbol>> &symbols) const
    -> std::optional<std::vector<SymbolPriceTick>> {
  return std::any_cast<std::optional<std::vector<SymbolPriceTick>>>(Execute(
      SelectSymbolPriceTicksEndpoint(),
      {{"limit",
        stonks::json::Any{.value = limit,
                          .converter = stonks::json::JsonConverter<int>{}}},
       {"period",
        stonks::json::Any{
            .value = period,
            .converter = stonks::json::JsonConverter<std::vector<Symbol>>{}}},
       {"symbols", stonks::json::Any{.value = symbols,
                                     .converter = stonks::json::JsonConverter<
                                         std::vector<SymbolPriceTick>>{}}}}));
}
}  // namespace stonks::finance