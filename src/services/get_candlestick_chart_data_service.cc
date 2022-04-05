#include "get_candlestick_chart_data_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

#include "finance_api.h"
#include "google_charts_conversions.h"
#include "utils.h"

namespace {
void HandleGetRequest(const web::http::http_request &request) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());
  const auto candlesticks = stonks::finance::GetCandlesticks(
      "ETHUSDT", stonks::finance::Interval::k1Hour,
      stonks::utils::ParseUnixTimeFromString("1 Mar 2022 00:00:00"),
      stonks::utils::ParseUnixTimeFromString("2 Mar 2022 00:00:00"));

  if (!candlesticks.has_value()) {
    request.reply(web::http::status_codes::NotFound);
    spdlog::info("Sent NotFound response");
    return;
  }

  request.reply(web::http::status_codes::OK,
                stonks::ConvertToJson(*candlesticks));
  spdlog::info("Sent OK response");
}
}  // namespace

namespace stonks {
pplx::task<void> GetCandlestickChartDataService::Start() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/candlestick_chart_data"};
  http_listener_.support(web::http::methods::GET, HandleGetRequest);
  return http_listener_.open();
}

pplx::task<void> GetCandlestickChartDataService::Stop() {
  return http_listener_.close();
}
}  // namespace stonks