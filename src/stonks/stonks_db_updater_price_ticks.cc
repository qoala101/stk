#include "stonks_db_updater_price_ticks.h"

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <exception>
#include <functional>
#include <memory>
#include <stop_token>
#include <string_view>
#include <thread>

#include "binance_settings.h"
#include "finance_conversions.h"
#include "json_conversions.h"
#include "web_socket.h"

namespace stonks {
namespace {
auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("DbUpdaterPriceTicks");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

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

class DbUpdaterPriceTicks::Impl {
 public:
  explicit Impl(std::shared_ptr<StonksDb> stonks_db)
      : web_socket_{binance::settings::GetBaseStreamUri()},
        stonks_db_{std::move(stonks_db)} {
    web_socket_.AppendUri("ws");
    web_socket_.Connect();
    web_socket_.SendMessage(kBookTickerSubscribeMessage);

    thread_ = std::jthread{std::bind_front(&Impl::Execute, this)};
  }

  Impl(const Impl &) = delete;
  Impl(Impl &&) noexcept = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) noexcept -> Impl & = default;

  ~Impl() {
    thread_.request_stop();
    thread_.join();

    web_socket_.SendMessage(kBookTickerUnsubscribeMessage);
    web_socket_.Disconnect();
  }

  void Execute(const std::stop_token &stop_token) {
    web_socket_
        .ReceiveMessages([this, &stop_token](const web::json::value &json) {
          return Handler(json, stop_token);
        })
        .wait();
  }

  auto Handler(const web::json::value &json, const std::stop_token &stop_token)
      -> network::WebSocket::ReceiveMessagesResult {
    auto symbol_book_ticker = binance::SymbolBookTicker{};

    try {
      symbol_book_ticker =
          stonks::json::ParseFromJson<binance::SymbolBookTicker>(json);
    } catch (const std::exception &exception) {
      Logger().error(exception.what());
      return stop_token.stop_requested()
                 ? network::WebSocket::ReceiveMessagesResult::kStop
                 : network::WebSocket::ReceiveMessagesResult::kContinue;
    }

    const auto new_prices =
        Prices{.best_ask_price = symbol_book_ticker.best_ask_price,
               .best_bid_price = symbol_book_ticker.best_bid_price};
    auto last_symbol_prices =
        last_symbol_prices_map_.find(symbol_book_ticker.symbol);

    auto new_prices_different = false;

    if (last_symbol_prices == last_symbol_prices_map_.end()) {
      new_prices_different = true;
      last_symbol_prices_map_[symbol_book_ticker.symbol] = new_prices;
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
      const auto symbol_price_tick =
          finance::ParseSymbolPriceTickFromBinanceSymbolBookTicker(
              symbol_book_ticker);
      stonks_db_->InsertSymbolPriceTick(symbol_price_tick);
    }

    return stop_token.stop_requested()
               ? network::WebSocket::ReceiveMessagesResult::kStop
               : network::WebSocket::ReceiveMessagesResult::kContinue;
  }

 private:
  struct Prices {
    double best_ask_price{};
    double best_bid_price{};
  };

  network::WebSocket web_socket_;
  std::shared_ptr<StonksDb> stonks_db_{};
  std::map<std::string, Prices> last_symbol_prices_map_{};
  std::jthread thread_{};
};

DbUpdaterPriceTicks::DbUpdaterPriceTicks(std::shared_ptr<StonksDb> stonks_db)
    : impl_{std::make_unique<Impl>(std::move(stonks_db))} {}

DbUpdaterPriceTicks::DbUpdaterPriceTicks(DbUpdaterPriceTicks &&) noexcept =
    default;

auto DbUpdaterPriceTicks::operator=(DbUpdaterPriceTicks &&) noexcept
    -> DbUpdaterPriceTicks & = default;

DbUpdaterPriceTicks::~DbUpdaterPriceTicks() = default;
}  // namespace stonks