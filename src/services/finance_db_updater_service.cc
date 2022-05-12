#include "finance_db_updater_service.h"

#include <spdlog/spdlog.h>

#include "finance_db.h"
#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"

namespace stonks {
namespace {
void SendSubscribeToPriceTicksRequest() {
  auto json = json::ConvertToJson(finance::PriceTicksServiceSubscribeRequest{
      .subscriber_uri = "http://localhost:6506/api/finance_db/new_price_tick"});

  rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/price_ticks/subscribe")
      .SetJson(json)
      .SendAndGetResponse();
}

void HandlePostRequest(const web::http::http_request &request,
                       finance::FinanceDb &finance_db) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/new_price_tick") {
    auto price_tick = json::ParseFromJson<finance::SymbolPriceTick>(json);

    if (!price_tick.has_value()) {
      spdlog::error("Cannot parse price tick");
      request.reply(web::http::status_codes::BadRequest);
      return;
    }

    request.reply(web::http::status_codes::OK);

    if (const auto failure = !finance_db.InsertSymbolPriceTick(*price_tick)) {
      spdlog::error("Cannot insert price tick to DB");
    }
  }

  request.reply(web::http::status_codes::NotFound);
}
}  // namespace

FinanceDbUpdaterService::FinanceDbUpdaterService()
    : finance_db_{"FinanceDbUpdaterService.db"} {}

auto FinanceDbUpdaterService::Start() -> pplx::task<void> {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/finance_db"};

  const auto post_handler =
      [&finance_db = finance_db_](const web::http::http_request &request) {
        HandlePostRequest(request, finance_db);
      };
  http_listener_.support(web::http::methods::POST, post_handler);
  http_listener_.open().wait();
  SendSubscribeToPriceTicksRequest();

  return pplx::create_task([]() {});
}

auto FinanceDbUpdaterService::Stop() -> pplx::task<void> {
  return http_listener_.close();
}
}  // namespace stonks