// clang-format off
#include "common_json_conversions.h"
// clang-format on

#include "service_symbol_prices_streams_controller.h"

namespace stonks::service {
auto SymbolPricesStreamsController::GetSymbolForStream() const
    -> cppcoro::task<core::Symbol> {
  co_return co_await Call<&Target::GetSymbolForStream>();
}
}  // namespace stonks::service