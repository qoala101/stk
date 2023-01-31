#include "core_sdb_proxy.h"

namespace stonks::core::sdb {
Proxy::Proxy(cpp::NnUp<ISymbolsDb> symbols_db)
    : symbols_db_{std::move(symbols_db)} {}

auto Proxy::SelectAssets() const -> cppcoro::task<std::vector<Asset>> {
  co_return co_await symbols_db_->SelectAssets();
}

auto Proxy::UpdateAssets(std::vector<Asset> assets) -> cppcoro::task<> {
  co_await symbols_db_->UpdateAssets(std::move(assets));
}

auto Proxy::SelectSymbolsWithPriceRecords() const
    -> cppcoro::task<std::vector<Symbol>> {
  co_return co_await symbols_db_->SelectSymbolsWithPriceRecords();
}

auto Proxy::SelectSymbolInfo(Symbol symbol) const
    -> cppcoro::task<cpp::Opt<SymbolInfo>> {
  co_return co_await symbols_db_->SelectSymbolInfo(std::move(symbol));
}

auto Proxy::SelectSymbolsInfo() const
    -> cppcoro::task<std::vector<SymbolInfo>> {
  co_return co_await symbols_db_->SelectSymbolsInfo();
}

auto Proxy::UpdateSymbolsInfo(std::vector<SymbolInfo> infos)
    -> cppcoro::task<> {
  co_await symbols_db_->UpdateSymbolsInfo(std::move(infos));
}

auto Proxy::SelectSymbolPriceRecords(const Symbol &symbol,
                                     const cpp::Opt<TimeOrder> &order,
                                     const cpp::Opt<absl::Time> &start_time,
                                     const cpp::Opt<absl::Time> &end_time,
                                     const cpp::Opt<int> &limit) const
    -> cppcoro::task<std::vector<SymbolPriceRecord>> {
  co_return co_await symbols_db_->SelectSymbolPriceRecords(
      symbol, order, start_time, end_time, limit);
}

auto Proxy::InsertSymbolPriceRecord(SymbolPriceRecord record)
    -> cppcoro::task<> {
  co_await symbols_db_->InsertSymbolPriceRecord(std::move(record));
}

auto Proxy::DeleteSymbolPriceRecords(const cpp::Opt<absl::Time> &start_time,
                                     const cpp::Opt<absl::Time> &end_time)
    -> cppcoro::task<> {
  co_await symbols_db_->DeleteSymbolPriceRecords(start_time, end_time);
}
}  // namespace stonks::core::sdb