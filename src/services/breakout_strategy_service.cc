#include "breakout_strategy_service.h"

#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"
#include "utils.h"

namespace stonks {
pplx::task<void> BreakoutStrategyService::Start() {
  service_state_ = true;

  return pplx::create_task([&thread = thread_,
                            &service_state = service_state_]() {
    thread = std::thread{[&service_state]() {
      while (service_state) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto json = ConvertToJson(finance::StrategyOrderRequest{
            .strategy_info = finance::StrategyInfo{.name = "breakout"},
            .order_request = finance::OrderRequest{
                .time = utils::GetUnixTime(),
                .action = finance::Action::kBuy,
                .symbol =
                    finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"},
                .quantity = 10 * static_cast<double>(std::rand()) / RAND_MAX,
                .price = 100 * static_cast<double>(std::rand()) / RAND_MAX}});

        rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
            .AppendUri("/api/order_proxy/order")
            .SetJson(json)
            .SendAndGetResponse();
      }
    }};
  });
}

pplx::task<void> BreakoutStrategyService::Stop() {
  service_state_ = false;

  return pplx::create_task([&thread = thread_]() { thread.join(); });
}
}  // namespace stonks