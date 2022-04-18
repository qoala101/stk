#include "breakout_strategy_service.h"

#include <spdlog/spdlog.h>

#include "breakout_strategy.h"
#include "buffered_candlesticks_stream.h"
#include "candlesticks_stream.h"
#include "finance_api.h"
#include "finance_enums.h"
#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"
#include "utils.h"

namespace {
void SendOrderRequest(stonks::finance::Symbol symbol,
                      stonks::finance::OrderRequest order_request) {
  order_request.symbol = std::move(symbol);

  auto json = stonks::ConvertToJson(stonks::finance::StrategyOrderRequest{
      .strategy_info = stonks::finance::StrategyInfo{.name = "breakout"},
      .order_request = std::move(order_request)});

  stonks::rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/order_proxy/order")
      .SetJson(json)
      .SendAndGetResponse();
}
}  // namespace

namespace stonks {
pplx::task<void> BreakoutStrategyService::Start() {
  service_state_ = true;

  return pplx::create_task(
      [&thread = thread_, &service_state = service_state_]() {
        thread = std::thread{[&service_state]() {
          const auto symbol =
              finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"};
          const auto interval = finance::Interval::k1Minute;
          auto strategy = finance::BreakoutStrategy{0};
          auto stream = finance::BufferedCandlesticksStream{
              finance::CandlesticksStream{
                  symbol, interval, utils::GetUnixTime() - std::chrono::days{2},
                  utils::GetUnixTime() - std::chrono::days{1}},
              1};

          while (service_state) {
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            const auto new_candlesticks = stream.GetNextCandlesticks();

            if (new_candlesticks.empty()) {
              spdlog::info("Breakout stretegy service is done");
              return;
            }

            const auto new_data_result =
                strategy.ProcessNewCandlesticks(new_candlesticks);

            if (new_data_result.has_value()) {
              SendOrderRequest(symbol, *new_data_result);
            }
          }
        }};
      });
}

pplx::task<void> BreakoutStrategyService::Stop() {
  service_state_ = false;

  return pplx::create_task([&thread = thread_]() { thread.join(); });
}
}  // namespace stonks