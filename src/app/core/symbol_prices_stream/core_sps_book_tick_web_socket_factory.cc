#include "core_sps_book_tick_web_socket_factory.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include <cppcoro/sync_wait.hpp>
#include <string>
#include <utility>

#include "cpp_typed_struct.h"

namespace stonks::core::sps {
namespace {
auto BookTickerEndpointFor [[nodiscard]] (const Symbol &symbol) {
  return network::WsEndpoint{
      fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                  absl::AsciiStrToLower(*symbol))};
}
}  // namespace

BookTickWebSocketFactory::BookTickWebSocketFactory(
    cpp::NnUp<ISymbolPricesStreamsController> controller,
    di::Factory<ISymbolsDb> symbols_db_factory,
    di::Factory<network::IWsClient> ws_client_factory)
    : controller_{std::move(controller)},
      symbols_db_factory_{std::move(symbols_db_factory)},
      ws_client_factory_{std::move(ws_client_factory)} {}

auto BookTickWebSocketFactory::Create() const
    -> cppcoro::task<networkx::WebSocket<&BookTickHandler::RecordAsPrice>> {
  const auto symbol = co_await controller_->GetSymbolForStream();
  const auto endpoint = BookTickerEndpointFor(symbol);

  co_return networkx::WebSocket<&BookTickHandler::RecordAsPrice>{
      endpoint, ws_client_factory_.Create(),
      sps::BookTickHandler{symbol, symbols_db_factory_.Create()}};
}
}  // namespace stonks::core::sps