#ifndef STONKS_FINANCE_DB_H_
#define STONKS_FINANCE_DB_H_

#include <memory>

#include "finance_types.h"

namespace stonks::finance {
class FinanceDb {
 public:
  explicit FinanceDb(std::string_view uri = "stonks.db");

  ~FinanceDb();

  std::optional<std::vector<std::string>> SelectAssets() const;

  std::optional<std::vector<Symbol>> SelectSymbols() const;

  bool InsertSymbolsPrices(const std::vector<SymbolPrices> &symbols_prices);

  std::optional<std::vector<TimeDouble>> SelectSymbolPrices(
      const Symbol &symbol) const;

  bool InsertSymbolBookTick(const SymbolBookTick &symbol_book_tick);

  std::optional<std::vector<SymbolBookTick>> SelectSymbolBookTicks(
      const Symbol &symbol) const;

  std::optional<Symbol> SelectSymbolById(int64_t symbol_id) const;

  std::optional<Symbol> SelectSymbolByName(std::string_view symbol_name) const;
  
  std::optional<int64_t> SelectSymbolIdBySymbol(const Symbol &symbol) const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_{};
};
}  // namespace stonks::finance

#endif  // STONKS_FINANCE_DB_H_
