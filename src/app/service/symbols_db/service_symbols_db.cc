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

auto SymbolsDb::SelectAssets() const -> std::vector<core::Asset> {
  return client_.Call<&core::ISymbolsDb::SelectAssets>();
}

void SymbolsDb::UpdateAssets(std::vector<core::Asset> assets) {
  client_.Call<&core::ISymbolsDb::UpdateAssets>(std::move(assets));
}

auto SymbolsDb::SelectSymbolsWithPriceRecords() const
    -> std::vector<core::Symbol> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolsWithPriceRecords>();
}

auto SymbolsDb::SelectSymbolInfo(core::Symbol symbol) const
    -> cpp::Opt<core::SymbolInfo> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolInfo>(std::move(symbol));
}

auto SymbolsDb::SelectSymbolsInfo() const -> std::vector<core::SymbolInfo> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolsInfo>();
}

void SymbolsDb::UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos) {
  client_.Call<&core::ISymbolsDb::UpdateSymbolsInfo>(std::move(infos));
}

auto SymbolsDb::SelectSymbolPriceRecords(const core::Symbol &symbol,
                                         const absl::Time *start_time,
                                         const absl::Time *end_time,
                                         const int *limit) const
    -> std::vector<core::SymbolPriceRecord> {
  return client_.Call<&core::ISymbolsDb::SelectSymbolPriceRecords>(
      symbol, start_time, end_time, limit);
}

void SymbolsDb::InsertSymbolPriceRecord(core::SymbolPriceRecord record) {
  client_.Call<&core::ISymbolsDb::InsertSymbolPriceRecord>(std::move(record));
}

void SymbolsDb::DeleteSymbolPriceRecords(absl::Time before_time) {
  client_.Call<&core::ISymbolsDb::DeleteSymbolPriceRecords>(before_time);
}
}  // namespace stonks::service