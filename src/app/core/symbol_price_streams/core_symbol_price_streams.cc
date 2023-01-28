#include "core_symbol_price_streams.h"

#include <coroutine>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "core_sps_stream_factory.h"
#include "cpp_share.h"

namespace stonks::core {
SymbolPriceStreams::SymbolPriceStreams(
    const std::vector<Symbol> &symbols, absl::Duration reattempt_interval,
    cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
    cpp::meta::ThreadSafe<cpp::Factory<network::IWsClient>> ws_client_factory)
    : stream_handles_{[&symbols, reattempt_interval,
                       symbols_db = cpp::Share(std::move(*symbols_db)),
                       &ws_client_factory]() {
        return symbols |
               ranges::views::transform(
                   [reattempt_interval, &symbols_db,
                    &ws_client_factory](const auto &symbol) {
                     return sps::StreamHandle{
                         symbol, reattempt_interval,
                         sps::StreamFactory{symbols_db, ws_client_factory}};
                   }) |
               ranges::to_vector;
      }()} {}
}  // namespace stonks::core