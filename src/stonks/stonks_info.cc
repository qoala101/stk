#include "stonks_info.h"

namespace stonks {
Info::Info() : finance_db_{"http://localhost:6505/Db"} {}

[[nodiscard]] auto Info::GetSymbols() const
    -> std::vector<finance::SymbolName> {
  return finance_db_.SelectSymbols();
}

[[nodiscard]] auto Info::GetStrategyNames() const -> std::vector<std::string> {
  return {"V1Strategy"};
}
}  // namespace stonks