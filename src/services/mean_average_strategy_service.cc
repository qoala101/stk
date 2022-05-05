#include "mean_average_strategy_service.h"

#include <spdlog/spdlog.h>

#include <gsl/util>

#include "finance_api.h"
#include "finance_types.h"
#include "google_charts_conversions.h"
#include "json_conversions.h"
#include "prices_stream.h"
#include "rest_request.h"

namespace stonks {
namespace {
void SendOrderRequest(finance::Symbol symbol,
                      finance::StrategyOrderRequest order_request) {
  order_request.symbol = std::move(symbol);

  auto json = json::ConvertToJson(order_request);

  rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/order_proxy/order")
      .SetJson(json)
      .SendAndGetResponse();
}

void SendSubscribeToOrderUpdatesRequest(boost::uuids::uuid order_uuid) {
  auto json =
      json::ConvertToJson(finance::StrategySubscribeToOrderUpdatesRequest{
          .order_uuid = order_uuid,
          .subscriber_uri =
              "http://localhost:6506/api/mean_average/order_update"});

  rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/order_proxy/subscribe")
      .SetJson(json)
      .SendAndGetResponse();
}

void HandleGetRequest(const web::http::http_request &request,
                      const finance::MeanAverageStrategy &strategy) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  request.reply(web::http::status_codes::OK,
                google_charts::ConvertToJson(strategy.GetAllPrices()));
  return;
}

void HandlePostRequest(const web::http::http_request &request,
                       finance::Symbol symbol,
                       finance::MeanAverageStrategy &strategy) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/order_update") {
    const auto order_update =
        json::ParseFromJson<finance::OrderProxyToStrategyOrderUpdate>(json);

    if (!order_update.has_value()) {
      spdlog::error("Cannot parse order update");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    const auto order_request = strategy.ProcessOrderUpdate(*order_update);

    if (order_request.has_value()) {
      SendOrderRequest(std::move(symbol), *order_request);
      SendSubscribeToOrderUpdatesRequest(order_request->order_uuid);
    }

    request.reply(web::http::status_codes::OK);
    return;
  }

  request.reply(web::http::status_codes::NotFound);
  return;
}
}  // namespace

MeanAverageStrategyService::MeanAverageStrategyService(finance::Symbol symbol,
                                                       double base_precision,
                                                       double price_precision,
                                                       double comission,
                                                       double profit)
    : symbol_{std::move(symbol)},
      strategy_{base_precision, price_precision, comission, profit} {}

pplx::task<void> MeanAverageStrategyService::Start() {
  service_state_.store(true, std::memory_order::relaxed);

  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/mean_average"};

  const auto get_request_handler =
      [&strategy = strategy_](const web::http::http_request &request) {
        HandleGetRequest(request, strategy);
      };
  http_listener_.support(web::http::methods::GET, get_request_handler);

  const auto post_request_handler =
      [&symbol = symbol_,
       &strategy = strategy_](const web::http::http_request &request) {
        HandlePostRequest(request, symbol, strategy);
      };
  http_listener_.support(web::http::methods::POST, post_request_handler);

  return http_listener_.open().then(
      [&symbol = symbol_, &service_state = service_state_, &thread = thread_,
       &strategy = strategy_]() {
        thread = std::thread{[&symbol, &service_state, &strategy]() {
          auto prices_stream = finance::PricesStream{symbol};
          prices_stream.Start();

          while (service_state.load(std::memory_order::relaxed)) {
            const auto new_prices = prices_stream.GetNextPrices();

            if (new_prices.empty()) {
              spdlog::info("Mean average strategy service is done");
              return;
            }

            const auto order_request = strategy.ProcessNewPrices(new_prices);

            if (order_request.has_value()) {
              SendOrderRequest(symbol, *order_request);
              SendSubscribeToOrderUpdatesRequest(order_request->order_uuid);
            }
          }
        }};
      });
}

pplx::task<void> MeanAverageStrategyService::Stop() {
  service_state_.store(false, std::memory_order::relaxed);

  return pplx::create_task([&thread = thread_]() { thread.join(); });
}
}  // namespace stonks