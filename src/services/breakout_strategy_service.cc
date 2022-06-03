#include "breakout_strategy_service.h"

#include <cpprest/http_msg.h>
#include <pplx/pplx.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <gsl/util>
#include <optional>
#include <utility>
#include <vector>

#include "breakout_strategy.h"
#include "buffered_candles_stream.h"
#include "candles_stream.h"
#include "finance_enums.h"
#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"
#include "utils.h"

namespace stonks {
namespace {
void SendOrderRequest(finance::Symbol symbol,
                      finance::StrategyOrderRequest order_request) {
  order_request.symbol = std::move(symbol);

  auto json = json::ConvertToJson(order_request);

  network::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/order_proxy/order")
      .SetJson(json)
      .SendAndGetResponse();
}
}  // namespace

BreakoutStrategyService::BreakoutStrategyService(finance::Symbol symbol)
    : symbol_{std::move(symbol)} {}

pplx::task<void> BreakoutStrategyService::Start() {
  service_state_.store(true, std::memory_order::relaxed);

  return pplx::create_task([&symbol = symbol_, &thread = thread_,
                            &service_state = service_state_]() {
    thread = std::thread{[&symbol, &service_state]() {
      const auto interval = finance::Interval::k1Minute;
      auto strategy = finance::BreakoutStrategy{};
      auto stream = finance::BufferedCandlesStream{
          finance::CandlesStream{symbol, interval,
                                 utils::GetUnixTime() - std::chrono::hours{5},
                                 utils::GetUnixTime()},
          1};

      while (service_state.load(std::memory_order::relaxed)) {
        const auto sleep_finally = gsl::finally(
            []() { std::this_thread::sleep_for(std::chrono::seconds{1}); });

        const auto new_candles = stream.GetNextCandles();

        if (new_candles.empty()) {
          spdlog::info("Breakout strategy service is done");
          return;
        }

        const auto new_data_result = strategy.ProcessNewCandles(new_candles);

        if (new_data_result.has_value()) {
          SendOrderRequest(symbol, *new_data_result);
        }
      }
    }};
  });
}

pplx::task<void> BreakoutStrategyService::Stop() {
  service_state_.store(false, std::memory_order::relaxed);

  return pplx::create_task([&thread = thread_]() { thread.join(); });
}
}  // namespace stonks