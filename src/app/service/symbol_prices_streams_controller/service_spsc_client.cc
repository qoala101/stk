// clang-format off
#include "service_json_conversions.h"
// clang-format on

#include "service_spsc_client.h"

namespace stonks::service {
auto ServiceClient<core::ISymbolPricesStreamsController>::GetSymbolForStream()
    const -> cppcoro::task<core::Symbol> {
  co_return co_await Call<&Target::GetSymbolForStream>();
}
}  // namespace stonks::service