// clang-format off
#include "common_json_conversions.h"
// clang-format on

#include "service_symbols_db.h"

#include <absl/time/time.h>

#include <coroutine>
#include <string>
#include <tuple>
#include <utility>

#include "network_json_common_conversions.h"
#include "networkx_client.h"

namespace stonks::service {
auto SymbolsDb::SelectAssets() const
    -> cppcoro::task<std::vector<core::Asset>> {
  co_return co_await Call<&Target::SelectAssets>();
}

auto SymbolsDb::UpdateAssets(std::vector<core::Asset> assets)
    -> cppcoro::task<> {
  co_await Call<&Target::UpdateAssets>(std::move(assets));
}

auto SymbolsDb::SelectSymbolsWithPriceRecords() const
    -> cppcoro::task<std::vector<core::Symbol>> {
  co_return co_await Call<&Target::SelectSymbolsWithPriceRecords>();
}

auto SymbolsDb::SelectSymbolInfo(core::Symbol symbol) const
    -> cppcoro::task<cpp::Opt<core::SymbolInfo>> {
  co_return co_await Call<&Target::SelectSymbolInfo>(std::move(symbol));
}

auto SymbolsDb::SelectSymbolsInfo() const
    -> cppcoro::task<std::vector<core::SymbolInfo>> {
  co_return co_await Call<&Target::SelectSymbolsInfo>();
}

auto SymbolsDb::UpdateSymbolsInfo(std::vector<core::SymbolInfo> infos)
    -> cppcoro::task<> {
  co_await Call<&Target::UpdateSymbolsInfo>(std::move(infos));
}

auto SymbolsDb::SelectSymbolPriceRecords(const core::Symbol &symbol,
                                         const absl::Time *start_time,
                                         const absl::Time *end_time,
                                         const int *limit) const
    -> cppcoro::task<std::vector<core::SymbolPriceRecord>> {
  co_return co_await Call<&Target::SelectSymbolPriceRecords>(symbol, start_time,
                                                             end_time, limit);
}

auto SymbolsDb::InsertSymbolPriceRecord(core::SymbolPriceRecord record)
    -> cppcoro::task<> {
  co_await Call<&Target::InsertSymbolPriceRecord>(std::move(record));
}

auto SymbolsDb::DeleteSymbolPriceRecords(const absl::Time *start_time,
                                         const absl::Time *end_time)
    -> cppcoro::task<> {
  co_await Call<&Target::DeleteSymbolPriceRecords>(start_time, end_time);
}
}  // namespace stonks::service