#include "finance_db_service.h"

#include <spdlog/spdlog.h>

#include <chrono>

#include "finance_db.h"
#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"

namespace stonks {
namespace {
void SendSubscribeToPriceTicksRequest() {
  auto json = json::ConvertToJson(finance::PriceTicksServiceSubscribeRequest{
      .subscriber_uri = "http://localhost:6506/api/finance_db/new_price_tick"});

  network::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/price_ticks/subscribe")
      .SetJson(json)
      .SendAndGetResponse();
}

void HandleGetRequest(const web::http::http_request &request,
                      finance::FinanceDb &finance_db) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/price_ticks") {
    const auto params_map =
        web::uri::split_query(request.request_uri().query());

    const auto limit_iter = params_map.find("limit");
    auto limit = std::optional<int>{std::nullopt};

    if (limit_iter != params_map.end()) {
      limit = std::stoll(limit_iter->second);
    }

    const auto start_time_iter = params_map.find("start_time");
    auto start_time = std::optional<std::chrono::milliseconds>{std::nullopt};

    if (start_time_iter != params_map.end()) {
      start_time =
          std::chrono::milliseconds{std::stoll(start_time_iter->second)};
    }

    const auto end_time_iter = params_map.find("end_time");
    auto end_time = std::optional<std::chrono::milliseconds>{std::nullopt};

    if (end_time_iter != params_map.end()) {
      end_time = std::chrono::milliseconds{std::stoll(end_time_iter->second)};
    }

    const auto price_ticks = finance_db.SelectSymbolPriceTicks(
        limit, finance::Period{.start_time = start_time, .end_time = end_time});

    if (!price_ticks.has_value()) {
      spdlog::error("Cannot get price ticks from DB");
      request.reply(web::http::status_codes::InternalError);
      return;
    }

    request.reply(web::http::status_codes::OK,
                  json::ConvertToJson(*price_ticks));
    return;
  }

  request.reply(web::http::status_codes::NotFound);
}

void HandlePostRequest(const web::http::http_request &request,
                       finance::FinanceDb &finance_db) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());

  const auto json = request.extract_json().get();
  const auto relative_uri = request.relative_uri().path();

  if (relative_uri == "/new_price_tick") {
    const auto price_tick =
        json::ParseFromJsonNoThrow<finance::SymbolPriceTick>(json);

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

FinanceDbService::FinanceDbService() : finance_db_{"FinanceDbService.db"} {}

auto FinanceDbService::Start() -> pplx::task<void> {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/finance_db"};

  const auto get_handler =
      [&finance_db = finance_db_](const web::http::http_request &request) {
        HandleGetRequest(request, finance_db);
      };
  http_listener_.support(web::http::methods::GET, get_handler);

  const auto post_handler =
      [&finance_db = finance_db_](const web::http::http_request &request) {
        HandlePostRequest(request, finance_db);
      };
  http_listener_.support(web::http::methods::POST, post_handler);

  http_listener_.open().wait();
  SendSubscribeToPriceTicksRequest();

  return pplx::create_task([]() {});
}

auto FinanceDbService::Stop() -> pplx::task<void> {
  return http_listener_.close();
}
}  // namespace stonks