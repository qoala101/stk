#include "get_candle_chart_data_service.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_msg.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include "ccutils_not_null.h"
#include <optional>

#include "finance_api.h"
#include "finance_enums.h"
#include "finance_types.h"
#include "google_charts_conversions.h"
#include "utils.h"

namespace stonks {
namespace {
void HandleGetRequest(const web::http::http_request &request) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());
  const auto candles = finance::GetCandles(
      finance::Symbol{.base_asset = "ETH", .quote_asset = "USDT"},
      finance::Interval::k1Minute, utils::GetUnixTime() - std::chrono::hours{1},
      utils::GetUnixTime());

  if (!candles.has_value()) {
    request.reply(web::http::status_codes::NotFound);
    spdlog::info("Sent NotFound response");
    return;
  }

  request.reply(web::http::status_codes::OK,
                google_charts::ConvertToJson(*candles));
  spdlog::info("Sent OK response");
}
}  // namespace

pplx::task<void> GetCandleChartDataService::Start() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/candle_chart_data"};
  http_listener_.support(web::http::methods::GET, HandleGetRequest);
  return http_listener_.open();
}

pplx::task<void> GetCandleChartDataService::Stop() {
  return http_listener_.close();
}
}  // namespace stonks