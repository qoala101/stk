#include "core_symbol_price_streams.h"

#include <cppcoro/sync_wait.hpp>
#include <optional>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <type_traits>
#include <utility>

namespace stonks::core {
SymbolPriceStreams::SymbolPriceStreams(
    std::vector<Symbol> symbols, absl::Duration reattempt_interval,
    const di::Factory<ISymbolsDb> &symbols_db_factory,
    const di::Factory<network::IWsClient> &ws_client_factory)
    : symbols_{std::move(symbols)},
      stream_handles_{[&symbols = symbols_, reattempt_interval,
                       &symbols_db_factory, &ws_client_factory]() {
        return symbols |
               ranges::views::transform(
                   [reattempt_interval, &symbols_db_factory,
                    &ws_client_factory](const auto &symbol) {
                     return sps::StreamHandle{
                         {symbol, symbols_db_factory, ws_client_factory},
                         reattempt_interval};
                   }) |
               ranges::to_vector;
      }()} {}

auto SymbolPriceStreams::GetStreamedSymbols() const
    -> cppcoro::task<std::vector<Symbol>> {
  co_return symbols_;
}
}  // namespace stonks::core