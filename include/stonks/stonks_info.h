#ifndef STONKS_STONKS_STONKS_INFO_H_
#define STONKS_STONKS_STONKS_INFO_H_

#include "finance_db_client_server.h"
#include "finance_types.h"

namespace stonks {
class Info {
 public:
  explicit Info();

  [[nodiscard]] auto GetSymbols() const -> std::vector<finance::SymbolName>;
  [[nodiscard]] auto GetStrategyNames() const -> std::vector<std::string>;
  [[nodiscard]] auto GetPriceTicks(const finance::SymbolName &symbol) const
      -> std::vector<finance::SymbolPriceTick>;

 private:
  finance::FinanceDbClient finance_db_;
};
}  // namespace stonks

#endif  // STONKS_STONKS_STONKS_INFO_H_
