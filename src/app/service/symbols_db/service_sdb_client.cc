// clang-format off
#include "service_json_conversions.h"
// clang-format on

#include "service_sdb_client.h"

#include <absl/time/time.h>

#include <coroutine>
#include <string>
#include <tuple>
#include <utility>

#include "network_json_common_conversions.h"
#include "networkx_client.h"

namespace stonks::service {
auto ServiceClient<core::ISymbolsDb>::SelectAssets() const
    -> cppcoro::task<std::vector<core::Asset>> {
  co_return co_await Call<&Target::SelectAssets>();
}

auto ServiceClient<core::ISymbolsDb>::UpdateAssets(
    std::vector<core::Asset> assets) -> cppcoro::task<> {
  co_await Call<&Target::UpdateAssets>(std::move(assets));
}

auto ServiceClient<core::ISymbolsDb>::SelectSymbolsWithPriceRecords() const
    -> cppcoro::task<std::vector<core::Symbol>> {
  co_return co_await Call<&Target::SelectSymbolsWithPriceRecords>();
}

auto ServiceClient<core::ISymbolsDb>::SelectSymbolInfo(
    core::Symbol symbol) const -> cppcoro::task<cpp::Opt<core::SymbolInfo>> {
  co_return co_await Call<&Target::SelectSymbolInfo>(std::move(symbol));
}

auto ServiceClient<core::ISymbolsDb>::SelectSymbolsInfo() const
    -> cppcoro::task<std::vector<core::SymbolInfo>> {
  co_return co_await Call<&Target::SelectSymbolsInfo>();
}

auto ServiceClient<core::ISymbolsDb>::UpdateSymbolsInfo(
    std::vector<core::SymbolInfo> infos) -> cppcoro::task<> {
  co_await Call<&Target::UpdateSymbolsInfo>(std::move(infos));
}

auto ServiceClient<core::ISymbolsDb>::SelectSymbolPriceRecords(
    const core::Symbol &symbol, const absl::Time *start_time,
    const absl::Time *end_time, const int *limit) const
    -> cppcoro::task<std::vector<core::SymbolPriceRecord>> {
  co_return co_await Call<&Target::SelectSymbolPriceRecords>(symbol, start_time,
                                                             end_time, limit);
}

auto ServiceClient<core::ISymbolsDb>::InsertSymbolPriceRecord(
    core::SymbolPriceRecord record) -> cppcoro::task<> {
  co_await Call<&Target::InsertSymbolPriceRecord>(std::move(record));
}

auto ServiceClient<core::ISymbolsDb>::DeleteSymbolPriceRecords(
    const absl::Time *start_time, const absl::Time *end_time)
    -> cppcoro::task<> {
  co_await Call<&Target::DeleteSymbolPriceRecords>(start_time, end_time);
}
}  // namespace stonks::service