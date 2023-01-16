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
SymbolPriceStreams::SymbolPriceStreams(std::vector<Symbol> symbols,
                                       absl::Duration reattempt_interval,
                                       sps::StreamFactory stream_factory)
    : symbols_{std::move(symbols)},
      stream_handles_{
          [&symbols = symbols_, reattempt_interval,
           web_socket_factory = cpp::Share(std::move(stream_factory))]() {
            return symbols |
                   ranges::views::transform(
                       [reattempt_interval,
                        &web_socket_factory](const auto &symbol) {
                         return sps::StreamHandle{symbol, reattempt_interval,
                                                  web_socket_factory};
                       }) |
                   ranges::to_vector;
          }()} {}

auto SymbolPriceStreams::GetStreamedSymbols() const
    -> cppcoro::task<std::vector<Symbol>> {
  co_return symbols_;
}
}  // namespace stonks::core