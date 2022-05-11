#include "web_socket.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "binance_settings.h"
#include "binance_types.h"
#include "json_conversions.h"

namespace {
const auto kBookTickerSubscribeMessage = R"(
{
  "method": "SUBSCRIBE",
  "params":
  [
  "!bookTicker"
  ],
  "id": 1
})";

const auto kBookTickerUnsubscribeMessage = R"(
{
  "method": "UNSUBSCRIBE",
  "params":
  [
  "!bookTicker"
  ],
  "id": 1
})";

TEST(WebSocket, Test) {
  auto web_socket =
      stonks::rest::WebSocket{stonks::binance::settings::GetBaseStreamUri()};
  web_socket.AppendUri("ws");
  web_socket.Connect();
  web_socket.SendMessage(kBookTickerSubscribeMessage);

  const auto num_ticks = 10;
  auto symbol_book_ticks = std::vector<stonks::binance::SymbolBookTicker>{};
  symbol_book_ticks.reserve(num_ticks);

  auto handler = [&symbol_book_ticks](const web::json::value &json) mutable {
    const auto symbol_book_ticker =
        stonks::json::ParseFromJson<stonks::binance::SymbolBookTicker>(json);

    if (symbol_book_ticker.has_value()) {
      symbol_book_ticks.emplace_back(*symbol_book_ticker);
    }

    return symbol_book_ticks.size() < num_ticks;
  };

  web_socket.ReceiveMessages(std::move(handler)).wait();
  web_socket.SendMessage(kBookTickerUnsubscribeMessage);
  web_socket.Disconnect();

  EXPECT_EQ(symbol_book_ticks.size(), num_ticks);
}
}  // namespace