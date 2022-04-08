#include "order_proxy_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

#include "google_charts_conversions.h"
#include "json_conversions.h"

namespace {
void HandleGetRequest(const web::http::http_request &request,
                      stonks::finance::OrderProxy &order_proxy) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto params_map = web::uri::split_query(request.request_uri().query());
  const auto drop_first_iter = params_map.find("drop_first");
  auto drop_first = 0;

  if (drop_first_iter != params_map.end()) {
    drop_first = std::stoi(drop_first_iter->second);
  }

  const auto records = order_proxy.FindRecords(
      "breakout",
      stonks::finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"},
      drop_first);
  request.reply(web::http::status_codes::OK, stonks::ConvertToJson(records));
}

void HandlePostRequest(const web::http::http_request &request,
                       stonks::finance::OrderProxy &order_proxy) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  auto strategy_order_request =
      stonks::ParseFromJson<stonks::finance::StrategyOrderRequest>(json);

  if (!strategy_order_request.has_value()) {
    spdlog::error("Cannot parse strategy order request");
    request.reply(web::http::status_codes::BadRequest);
    return;
  }

  order_proxy.RecordStrategyOrderRequest(std::move(*strategy_order_request));
  request.reply(web::http::status_codes::OK);
}
}  // namespace

namespace stonks {
pplx::task<void> OrderProxyService::Start() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/order_proxy/order"};

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