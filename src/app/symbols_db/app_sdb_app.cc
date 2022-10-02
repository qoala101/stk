#include "app_sdb_app.h"

#include <absl/time/time.h>
#include <fmt/core.h>

#include "core_types.h"

namespace stonks::app::sdb {
App::App() {}

void App::InsertSymbolPriceRecord(const core::SymbolPriceRecord &record) {
  fmt::print("Received: {} {} {}", absl::FormatTime(record.time),
             record.symbol.value, record.price);
}
}  // namespace stonks::app::sdb