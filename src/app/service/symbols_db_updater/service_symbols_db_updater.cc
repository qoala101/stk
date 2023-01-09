#include "service_symbols_db_updater.h"

#include <absl/time/time.h>

#include <coroutine>

namespace stonks::service {
auto SymbolsDbUpdater::GetUpdateSymbolsInfoInterval() const
    -> cppcoro::task<absl::Duration> {
  co_return co_await Call<&Target::GetUpdateSymbolsInfoInterval>();
}
}  // namespace stonks::service