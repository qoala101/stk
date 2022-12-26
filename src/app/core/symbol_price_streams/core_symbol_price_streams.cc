#include "core_symbol_price_streams.h"

#include <cppcoro/sync_wait.hpp>
#include <optional>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <type_traits>
#include <utility>

namespace stonks::core {
SymbolPriceStreams::SymbolPriceStreams(
    const std::vector<Symbol> &symbols, absl::Duration reattempt_interval,
    const di::Factory<ISymbolsDb> &symbols_db_factory,
    const di::Factory<network::IWsClient> &ws_client_factory)
    : stream_handles_{[&symbols, reattempt_interval, &symbols_db_factory,
                       &ws_client_factory]() {
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
}  // namespace stonks::core