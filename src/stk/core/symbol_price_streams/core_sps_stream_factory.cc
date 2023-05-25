/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#include "core_sps_stream_factory.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include <not_null.hpp>
#include <string>
#include <utility>

#include "core_sps_price_recorder.h"
#include "cpp_typed_struct.h"
#include "network_ws_types.h"

namespace vh::stk::core::sps {
namespace {
auto BookTickerEndpointFor [[nodiscard]] (const Symbol &symbol) {
  return network::WsEndpoint{
      fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                  absl::AsciiStrToLower(*symbol))};
}
}  // namespace

StreamFactory::StreamFactory(
    cpp::NnSp<ISymbolsDb> symbols_db,
    cpp::NnSp<cpp::Factory<network::IWsClient>> ws_client_factory)
    : symbols_db_{std::move(symbols_db)},
      ws_client_factory_{std::move(ws_client_factory)} {}

auto StreamFactory::Create(Symbol symbol)
    -> networkx::WebSocket<&PriceRecorder::RecordAsPrice> {
  return {BookTickerEndpointFor(symbol), ws_client_factory_->Create(),
          sps::PriceRecorder{std::move(symbol), symbols_db_}};
}
}  // namespace vh::stk::core::sps