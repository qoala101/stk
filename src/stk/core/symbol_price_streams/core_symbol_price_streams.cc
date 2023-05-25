/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "core_symbol_price_streams.h"

#include <memory>
#include <not_null.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/functional/invoke.hpp>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <type_traits>
#include <utility>

#include "core_sps_stream_factory.h"
#include "cpp_share.h"
#include "cpp_typed_struct.h"

namespace vh::stk::core {
SymbolPriceStreams::SymbolPriceStreams(
    const std::vector<Symbol> &symbols, absl::Duration reattempt_interval,
    cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
    cpp::meta::ThreadSafe<cpp::Factory<network::IWsClient>> ws_client_factory)
    : stream_handles_{
          [&symbols, reattempt_interval,
           symbols_db = cpp::Share(std::move(*symbols_db)),
           ws_client_factory = cpp::Share(std::move(*ws_client_factory))]() {
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

SymbolPriceStreams::~SymbolPriceStreams() {
  const auto disconnect_tasks =
      stream_handles_ | ranges::views::transform([](const auto &stream_handle) {
        return stream_handle.Disconnect();
      }) |
      ranges::to_vector;

  for (const auto &task : disconnect_tasks) {
    task.wait();
  }
}
}  // namespace vh::stk::core