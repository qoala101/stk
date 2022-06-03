
#include "price_ticks_db_stream.h"

#include <bits/exception.h>
#include <cpprest/base_uri.h>
#include <cpprest/http_msg.h>
#include <fmt/format.h>
#include <pplx/pplxtasks.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <condition_variable>
#include <gsl/assert>
#include <memory>
#include <mutex>
#include <optional>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/view.hpp>
#include <ratio>
#include <string>
#include <utility>

#include "finance_types.h"
#include "json_conversions.h"
#include "rest_request.h"

namespace stonks::finance {
namespace {
auto SendGetPriceTicksRequest(
    std::optional<int> limit = std::nullopt,
    const std::optional<Period> &period = std::nullopt,
    const std::optional<std::vector<Symbol>> &symbols = std::nullopt)
    -> std::optional<std::vector<SymbolPriceTick>> {
  auto request =
      network::RestRequest{web::http::methods::GET, "http://localhost:6506"}
          .AppendUri("/api/finance_db/price_ticks")
          .AddParameter("limit", limit);

  if (period.has_value()) {
    request.AddParameter("start_time", period->start_time);
    request.AddParameter("end_time", period->end_time);
  }

  const auto response = request.SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get price ticks from DB");
    return std::nullopt;
  }

  return json::ParseFromJson<std::vector<SymbolPriceTick>>(*response);
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
    auto price_tick =
        json::ParseFromJsonNoThrow<finance::SymbolPriceTick>(json);

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

PriceTicksDbStream::PriceTicksDbStream(const std::optional<Period> &period,
                                       int buffer_size)
    : period_{period}, buffer_size_{buffer_size} {
  Expects(buffer_size > 0);
}

PriceTicksDbStream::~PriceTicksDbStream() = default;

auto PriceTicksDbStream::GetNextPriceTicks()
    -> std::optional<std::vector<SymbolPriceTick>> {
  if (buffer_.empty()) {
    auto period = Period{};

    if (last_time_.has_value()) {
      period.start_time = *last_time_ + std::chrono::milliseconds{1};
      period.end_time = period_.has_value() ? period_->end_time : std::nullopt;
    }

    auto price_ticks = SendGetPriceTicksRequest(buffer_size_, period);

    if (!price_ticks.has_value()) {
      spdlog::error("Cannot get price ticks from DB");
      return std::nullopt;
    }

    if (price_ticks->empty()) {
      return {};
    }

    buffer_ = std::move(*price_ticks);
  }

  const auto result = std::vector<SymbolPriceTick>{buffer_[next_buffer_index_]};
  ++next_buffer_index_;

  if (const auto next_index_out_of_buffer =
          next_buffer_index_ >= buffer_.size()) {
    last_time_ = buffer_.back().time;
    buffer_.clear();
    next_buffer_index_ = 0;
  }

  return result;
}
}  // namespace stonks::finance