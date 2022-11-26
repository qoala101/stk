// clang-format off
#include "service_json_conversions.h"
// clang-format on

#include "service_symbols_db.h"

#include <absl/time/time.h>

#include <string>
#include <utility>

#include "networkx_client.h"

namespace stonks::service {
SymbolsDb::SymbolsDb(networkx::Client<core::ISymbolsDb> client)
    : client_{std::move(client)} {}

auto SymbolsDb::SelectAssets() const
    -> cppcoro::task<std::vector<core::Asset>> {
  return client_.Call<&core::ISymbolsDb::SelectAssets>();
}

auto SymbolsDb::UpdateAssets(std::vector<core::Asset> assets)
    -> cppcoro::task<> {
  return client_.Call<&core::ISymbolsDb::UpdateAssets>(std::move(assets));
}

auto SymbolsDb::SelectSymbolsWithPriceRecords() const
    -> cppcoro::task<std::vector<core::Symbol>> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolsWithPriceRecords>();
}

auto SymbolsDb::SelectSymbolInfo(core::Symbol symbol) const
    -> cppcoro::task<cpp::Opt<core::SymbolInfo>> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolInfo>(std::move(symbol));
}

auto SymbolsDb::SelectSymbolsInfo() const
    -> cppcoro::task<std::vector<core::SymbolInfo>> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolsInfo>();
}

auto SymbolsDb::UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos)
    -> cppcoro::task<> {
  return client_.Call<&core::ISymbolsDb::UpdateSymbolsInfo>(std::move(infos));
}

auto SymbolsDb::SelectSymbolPriceRecords(const core::Symbol &symbol,
                                         const absl::Time *start_time,
                                         const absl::Time *end_time,
                                         const int *limit) const
    -> cppcoro::task<std::vector<core::SymbolPriceRecord>> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolPriceRecords>(
      symbol, start_time, end_time, limit);
}

auto SymbolsDb::InsertSymbolPriceRecord(core::SymbolPriceRecord record)
    -> cppcoro::task<> {
  return client_.Call<&core::ISymbolsDb::InsertSymbolPriceRecord>(
      std::move(record));
}

auto SymbolsDb::DeleteSymbolPriceRecords(absl::Time before_time)
    -> cppcoro::task<> {
  return client_.Call<&core::ISymbolsDb::DeleteSymbolPriceRecords>(before_time);
}
}  // namespace stonks::service