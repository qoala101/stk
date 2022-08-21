#include "order_monitor_service.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_msg.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <optional>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/view.hpp>
#include <string>
#include <utility>
#include <vector>

#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"

namespace stonks {
namespace {
void HandlePostRequest(const web::http::http_request &request,
                       finance::OrderMonitor &order_monitor) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/order") {
    auto order_request =
        json::ParseFromJsonNoThrow<finance::OrderProxyOrderRequest>(json);

    if (!order_request.has_value()) {
      spdlog::error("Cannot parse order request");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    order_monitor.ProcessOrderRequest(std::move(*order_request));
    request.reply(web::http::status_codes::OK);
    return;
  }

  if (relative_uri == "/monitor") {
    auto monitor_requests = json::ParseFromJsonNoThrow<
        std::vector<finance::OrderProxyMonitorRequest>>(json);

    if (!monitor_requests.has_value()) {
      spdlog::error("Cannot parse monitor request");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    order_monitor.MonitorOrders(*monitor_requests);
    request.reply(web::http::status_codes::OK);
    return;
  }

  request.reply(web::http::status_codes::NotFound);
}

void HandleOrdersUpdated(
    const std::vector<finance::OrderMonitorOrderUpdate> &order_updates) {
  network::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/order_proxy/update_orders")
      .SetJson(json::ConvertToJson(order_updates))
      .SendAndGetResponse();
}
}  // namespace

OrderMonitorService::OrderMonitorService()
    : order_monitor_{HandleOrdersUpdated} {}

pplx::task<void> OrderMonitorService::Start() {
  order_monitor_.Start();

  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/order_monitor"};
  const auto post_request_handler =
      [&order_monitor =
           order_monitor_](const web::http::http_request &request) {
        HandlePostRequest(request, order_monitor);
      };
  http_listener_.support(web::http::methods::POST, post_request_handler);
  return http_listener_.open();
}

pplx::task<void> OrderMonitorService::Stop() {
  order_monitor_.Stop();
  return http_listener_.close();
}
}  // namespace stonks