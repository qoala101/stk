#include "price_ticks_service.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <future>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "binance_settings.h"
#include "binance_types.h"
#include "finance_conversions.h"
#include "finance_db.h"
#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"
#include "web_socket.h"

namespace stonks {
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

void SendPriceTickToUri(const finance::SymbolPriceTick &price_tick,
                        std::string_view uri) {
  network::RestRequest{web::http::methods::POST, uri}
      .SetJson(json::ConvertToJson(price_tick))
      .SendAndGetResponse();
}

void HandlePostRequest(
    const web::http::http_request &request,
    std::vector<finance::PriceTicksServiceSubscribeRequest> &subscribers,
    std::mutex &subscribers_mutex) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/subscribe") {
    auto subscribe_request =
        json::ParseFromJsonNoThrow<finance::PriceTicksServiceSubscribeRequest>(
            json);

    if (!subscribe_request.has_value()) {
      spdlog::error("Cannot parse order subscribe request");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    {
      const auto lock = std::lock_guard{subscribers_mutex};

      subscribers.emplace_back(std::move(*subscribe_request));
    }

    request.reply(web::http::status_codes::OK);
    return;
  }

  request.reply(web::http::status_codes::NotFound);
}
}  // namespace

PriceTicksService::PriceTicksService() : finance_db_{":memory:"} {}

auto PriceTicksService::Start() -> pplx::task<void> {
  keep_alive_.store(true, std::memory_order_relaxed);

  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/price_ticks"};

  const auto post_handler = [&subscribers = subscribers_,
                             &subscribers_mutex = subscribers_mutex_](
                                const web::http::http_request &request) {
    HandlePostRequest(request, subscribers, subscribers_mutex);
  };
  http_listener_.support(web::http::methods::POST, post_handler);

  thread_ = std::thread{[&subscribers = subscribers_,
                         &subscribers_mutex = subscribers_mutex_,
                         &finance_db = finance_db_,
                         &keep_alive = keep_alive_]() {
    auto web_socket = stonks::network::WebSocket{
        stonks::binance::settings::GetBaseStreamUri()};
    web_socket.AppendUri("ws");
    web_socket.Connect();
    web_socket.SendMessage(kBookTickerSubscribeMessage);

    struct Prices {
      double best_ask_price{};
      double best_bid_price{};
    };
    auto last_symbol_prices_map = std::map<std::string, Prices>{};

    auto handler = [&subscribers, &subscribers_mutex, &last_symbol_prices_map,
                    &finance_db,
                    &keep_alive](const web::json::value &json) mutable {
      {
        const auto lock = std::lock_guard{subscribers_mutex};

        if (subscribers.empty()) {
          return network::WebSocket::ReceiveMessagesResult::kContinue;
        }
      }

      const auto symbol_book_ticker =
          stonks::json::ParseFromJsonNoThrow<stonks::binance::SymbolBookTicker>(
              json);

      if (symbol_book_ticker.has_value()) {
        const auto new_prices =
            Prices{.best_ask_price = symbol_book_ticker->best_ask_price,
                   .best_bid_price = symbol_book_ticker->best_bid_price};
        auto last_symbol_prices =
            last_symbol_prices_map.find(symbol_book_ticker->symbol);

        auto new_prices_different = false;

        if (last_symbol_prices == last_symbol_prices_map.end()) {
          new_prices_different = true;
          last_symbol_prices_map[symbol_book_ticker->symbol] = new_prices;
        } else {
          new_prices_different = (last_symbol_prices->second.best_ask_price !=
                                  new_prices.best_ask_price) ||
                                 (last_symbol_prices->second.best_bid_price !=
                                  new_prices.best_bid_price);

          if (new_prices_different) {
            last_symbol_prices->second = new_prices;
          }
        }

        if (new_prices_different) {
          {
            const auto lock = std::lock_guard{subscribers_mutex};

            const auto symbol_price_tick =
                finance::ParseSymbolPriceTickFromBinanceSymbolBookTicker(
                    *symbol_book_ticker);

            for (const auto &subscriber : subscribers) {
              SendPriceTickToUri(symbol_price_tick, subscriber.subscriber_uri);
            }
          }
        }
      }

      return keep_alive.load(std::memory_order_relaxed)
                 ? network::WebSocket::ReceiveMessagesResult::kContinue
                 : network::WebSocket::ReceiveMessagesResult::kStop;
    };

    web_socket.ReceiveMessages(std::move(handler)).wait();
    web_socket.SendMessage(kBookTickerUnsubscribeMessage);
    web_socket.Disconnect();
  }};

  return http_listener_.open();
}

auto PriceTicksService::Stop() -> pplx::task<void> {
  keep_alive_.store(false, std::memory_order_relaxed);
  thread_.join();
  return http_listener_.close();
}
}  // namespace stonks