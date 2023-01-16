#include "core_sps_book_tick_web_socket_factory.h"

#include <absl/strings/ascii.h>
#include <fmt/core.h>

#include <coroutine>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "cpp_auto_updatable.h"
#include "cpp_message_exception.h"
#include "cpp_typed_struct.h"
#include "network_ws_types.h"

namespace stonks::core::sps {
namespace {
auto BookTickerEndpointFor [[nodiscard]] (const Symbol &symbol) {
  return network::WsEndpoint{
      fmt::format("wss://stream.binance.com:9443/ws/{}@bookTicker",
                  absl::AsciiStrToLower(*symbol))};
}

auto GetBaseAssetPriceStep(const Symbol &symbol, const ISymbolsDb &symbols_db)
    -> cppcoro::task<double> {
  auto symbol_info = co_await symbols_db.SelectSymbolInfo(symbol);

  if (!symbol_info.has_value()) {
    throw cpp::MessageException{
        fmt::format("Couldn't get symbol info for {}", *symbol)};
  }

  co_return symbol_info->base_asset.price_step;
}
}  // namespace

BookTickWebSocketFactory::BookTickWebSocketFactory(
    common::ThreadSafe<cpp::NnUp<ISymbolsDb>> symbols_db,
    common::ThreadSafe<cpp::NnUp<ISymbolsDbUpdater>> symbols_db_updater,
    common::ThreadSafe<di::Factory<network::IWsClient>> ws_client_factory)
    : symbols_db_{std::move(*symbols_db)},
      symbols_db_updater_{std::move(symbols_db_updater)},
      ws_client_factory_{std::move(ws_client_factory)} {}

auto BookTickWebSocketFactory::Create(Symbol symbol) const
    -> cppcoro::task<networkx::WebSocket<&BookTickHandler::RecordAsPrice>> {
  auto endpoint = BookTickerEndpointFor(symbol);

  const auto update_symbols_info_interval =
      co_await symbols_db_updater_->GetUpdateSymbolsInfoInterval();

  auto base_asset_price_step = cpp::AutoUpdatable<double>{
      [symbol, symbols_db = symbols_db_]() -> cppcoro::task<double> {
        return GetBaseAssetPriceStep(symbol, *symbols_db);
      },
      update_symbols_info_interval};

  auto last_price_record = co_await GetLastPriceRecord(symbol);

  co_return networkx::WebSocket<&BookTickHandler::RecordAsPrice>{
      std::move(endpoint), ws_client_factory_.Create(),
      sps::BookTickHandler{std::move(symbol), symbols_db_,
                           std::move(base_asset_price_step),
                           std::move(last_price_record)}};
}

auto BookTickWebSocketFactory::GetLastPriceRecord(const Symbol &symbol) const
    -> cppcoro::task<cpp::Opt<SymbolPriceRecord>> {
  const auto order = TimeOrder::kNewFirst;
  const auto limit = 1;

  auto records = co_await symbols_db_->SelectSymbolPriceRecords(
      symbol, &order, nullptr, nullptr, &limit);

  if (records.empty()) {
    co_return std::nullopt;
  }

  co_return std::move(records.front());
}
}  // namespace stonks::core::sps