#include "core_sps_stream_factory.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include <coroutine>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core_sps_price_recorder.h"
#include "cpp_typed_struct.h"
#include "network_ws_types.h"

namespace stonks::core::sps {
namespace {
auto BookTickerEndpointFor [[nodiscard]] (const Symbol &symbol) {
  return network::WsEndpoint{
      fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                  absl::AsciiStrToLower(*symbol))};
}
}  // namespace

StreamFactory::StreamFactory(
    cpp::meta::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
    cpp::meta::ThreadSafe<di::Factory<network::IWsClient>> ws_client_factory)
    : symbols_db_{std::move(*symbols_db)},
      ws_client_factory_{std::move(ws_client_factory)} {}

auto StreamFactory::Create(Symbol symbol) const
    -> networkx::WebSocket<&PriceRecorder::RecordAsPrice> {
  return {BookTickerEndpointFor(symbol), ws_client_factory_.Create(),
          sps::PriceRecorder{std::move(symbol), symbols_db_}};
}
}  // namespace stonks::core::sps