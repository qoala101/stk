#include "order_proxy_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

#include "google_charts_conversions.h"
#include "json_conversions.h"
#include "rest_request.h"

namespace stonks {
namespace {
void ForwardOrderRequestToProcessing(const finance::Order &order) {
  const auto order_request =
      finance::OrderProxyOrderRequest{.order_uuid = order.uuid,
                                      .symbol = order.symbol,
                                      .buy_or_sell = order.buy_or_sell,
                                      .amount = order.amount,
                                      .order_type = order.order_type};

  rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/order_monitor/order")
      .SetJson(json::ConvertToJson(order_request))
      .SendAndGetResponse();
}

void HandleGetRequest(const web::http::http_request &request,
                      finance::OrderProxy &order_proxy) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto params_map = web::uri::split_query(request.request_uri().query());
  const auto drop_first_iter = params_map.find("drop_first");
  auto drop_first = 0;

  if (drop_first_iter != params_map.end()) {
    drop_first = std::stoi(drop_first_iter->second);
  }

  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/strategy") {
    const auto records = order_proxy.FindOrderRequests(
        "breakout", finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"},
        drop_first);
    request.reply(web::http::status_codes::OK,
                  google_charts::ConvertToJson(records));
    return;
  }

  if (relative_uri == "/balance_history") {
    const auto balance_history =
        order_proxy.CalcBalanceHistory("USDT", std::nullopt, drop_first);
    request.reply(web::http::status_codes::OK,
                  google_charts::ConvertToJson(balance_history));
    return;
  }

  if (relative_uri == "/orders") {
    const auto orders = order_proxy.GetAllOrders(drop_first);
    request.reply(web::http::status_codes::OK,
                  google_charts::ConvertToJson(orders));
    return;
  }

  request.reply(web::http::status_codes::NotFound);
}

void HandlePostRequest(const web::http::http_request &request,
                       finance::OrderProxy &order_proxy) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/order") {
    auto strategy_order_request =
        json::ParseFromJson<finance::StrategyOrderRequest>(json);

    if (!strategy_order_request.has_value()) {
      spdlog::error("Cannot parse strategy order request");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    const auto &order =
        order_proxy.RecordStrategyOrderRequest(*strategy_order_request);
    ForwardOrderRequestToProcessing(order);

    request.reply(web::http::status_codes::OK);
    return;
  }

  if (relative_uri == "/update_orders") {
    auto order_updates =
        json::ParseFromJson<std::vector<finance::OrderMonitorOrderUpdate>>(
            json);

    if (!order_updates.has_value()) {
      spdlog::error("Cannot parse order update");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    order_proxy.UpdateOrders(*order_updates);

    request.reply(web::http::status_codes::OK);
    return;
  }

  request.reply(web::http::status_codes::NotFound);
}
}  // namespace

pplx::task<void> OrderProxyService::Start() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/order_proxy"};

  const auto get_handler =
      [&order_proxy = order_proxy_](const web::http::http_request &request) {
        HandleGetRequest(request, order_proxy);
      };
  http_listener_.support(web::http::methods::GET, get_handler);

  const auto post_handler =
      [&order_proxy = order_proxy_](const web::http::http_request &request) {
        HandlePostRequest(request, order_proxy);
      };
  http_listener_.support(web::http::methods::POST, post_handler);

  return http_listener_.open();
}

pplx::task<void> OrderProxyService::Stop() { return http_listener_.close(); }
}  // namespace stonks