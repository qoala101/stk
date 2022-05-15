#include "price_ticks_stream.h"

#include <spdlog/spdlog.h>

#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"

namespace stonks::finance {
namespace {
void SendSubscribeToPriceTicksRequest() {
  auto json = json::ConvertToJson(finance::PriceTicksServiceSubscribeRequest{
      .subscriber_uri = "http://localhost:6506/api/"
                        "price_ticks_stream_real_time/new_price_tick"});

  rest::RestRequest{web::http::methods::POST, "http://localhost:6506"}
      .AppendUri("/api/price_ticks/subscribe")
      .SetJson(json)
      .SendAndGetResponse();
}

void HandlePostRequest(const web::http::http_request &request,
                       std::vector<SymbolPriceTick> &price_ticks,
                       std::mutex &price_ticks_mutex,
                       std::condition_variable &price_ticks_cond_var) {
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

    {
      const auto lock = std::lock_guard{price_ticks_mutex};

      price_ticks.emplace_back(std::move(*price_tick));
      price_ticks_cond_var.notify_all();
    }

    return;
  }

  request.reply(web::http::status_codes::NotFound);
}
}  // namespace

PriceTicksStreamRealTime::PriceTicksStreamRealTime() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/price_ticks_stream_real_time"};

  const auto post_handler = [&price_ticks = price_ticks_,
                             &price_ticks_mutex = price_ticks_mutex_,
                             &price_ticks_cond_var = price_ticks_cond_var_](
                                const web::http::http_request &request) {
    HandlePostRequest(request, price_ticks, price_ticks_mutex,
                      price_ticks_cond_var);
  };
  http_listener_.support(web::http::methods::POST, post_handler);
  http_listener_.open().wait();
  SendSubscribeToPriceTicksRequest();
}

PriceTicksStreamRealTime::~PriceTicksStreamRealTime() {
  http_listener_.close().wait();
}

auto PriceTicksStreamRealTime::GetNextPriceTicks()
    -> std::vector<SymbolPriceTick> {
  const auto check_if_has_price_ticks = [&price_ticks = price_ticks_]() {
    return !price_ticks.empty();
  };

  auto price_ticks = std::vector<SymbolPriceTick>{};

  {
    auto lock = std::unique_lock{price_ticks_mutex_};
    price_ticks_cond_var_.wait(lock, check_if_has_price_ticks);

    price_ticks.swap(price_ticks_);
  }

  return price_ticks;
}
}  // namespace stonks::finance