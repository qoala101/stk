#include "stonks_info.h"

namespace stonks {
Info::Info() : finance_db_{"http://localhost:6506/Db"} {}

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