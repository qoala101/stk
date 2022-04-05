#include "binance_api.h"

#include <spdlog/spdlog.h>

#include <range/v3/to_container.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_enum_conversions.h"
#include "binance_settings.h"
#include "json_conversions.h"
#include "rest_request.h"
#include "utils.h"

namespace {
std::optional<std::vector<stonks::binance::Symbol>>
ParseSymbolsFromExchangeInfo(const web::json::value &exchange_info) {
  if (!exchange_info.has_field("symbols")) {
    return std::nullopt;
  }

  const auto &symbols_value = exchange_info.at("symbols");

  if (!symbols_value.is_array()) {
    return std::nullopt;
  }

  const auto &symbols_array = symbols_value.as_array();

  const auto has_usdt_quote_asset = [](const web::json::value &value) {
    if (!value.has_string_field("quoteAsset")) {
      return false;
    }

    return value.at("quoteAsset").as_string() == "USDT";
  };
  const auto has_symbol_field = [](const web::json::value &value) {
    return value.has_string_field("symbol");
  };
  const auto get_symbol_field = [](const web::json::value &value) {
    return value.at("symbol").as_string();
  };

  return symbols_array | ranges::views::filter(has_usdt_quote_asset) |
         ranges::views::filter(has_symbol_field) |
         ranges::views::transform(get_symbol_field) | ranges::to_vector;
}
}  // namespace

namespace stonks::binance {
std::optional<std::vector<Symbol>> GetSymbols() {
  const auto exchange_info =
      rest::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/exchangeInfo")
          .SendAndGetResponse();

  if (!exchange_info.has_value()) {
    spdlog::error("Cannot get symbols");
    return std::nullopt;
  }

  return ParseSymbolsFromExchangeInfo(*exchange_info);
}

std::optional<Balances> GetBalances() {
  auto request =
      rest::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/account")
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("timestamp", utils::GetUnixTime());
  const auto params = request.GetParametersAsString();
  const auto signed_params =
      utils::SignUsingHmacSha256(params, settings::GetSecretKey());
  const auto response =
      request.AddParameter("signature", signed_params).SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get balance");
    return std::nullopt;
  }

  return response->serialize();
}

std::optional<PlaceOrderResult> PlaceOrder(std::string_view symbol, Side side,
                                           Type type,
                                           std::optional<double> quantity,
                                           std::optional<double> price,
                                           TimeInForce time_in_force) {
  auto request =
      rest::RestRequest{web::http::methods::POST, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/order")
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("symbol", symbol)
          .AddParameter("side", side)
          .AddParameter("type", type)
          .AddParameter("timeInForce", time_in_force)
          .AddParameter("timestamp", utils::GetUnixTime())
          .AddParameter("quantity", quantity)
          .AddParameter("price", price);

  const auto params = request.GetParametersAsString();
  const auto signed_params =
      utils::SignUsingHmacSha256(params, settings::GetSecretKey());
  const auto response =
      request.AddParameter("signature", signed_params).SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot place order");
    return std::nullopt;
  }

  return ParseFromJson<PlaceOrderResult>(*response);
}

std::optional<std::vector<Kline>> GetKlines(
    std::string_view symbol, CandlestickInterval interval,
    std::optional<std::chrono::milliseconds> start_time,
    std::optional<std::chrono::milliseconds> end_time,
    std::optional<int> limit) {
  const auto response =
      rest::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/klines")
          .AddParameter("symbol", symbol)
          .AddParameter("interval", interval)
          .AddParameter("startTime", start_time)
          .AddParameter("endTime", end_time)
          .AddParameter("limit", limit)
          .SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get klines");
    return std::nullopt;
  }

  return ParseFromJson<std::vector<Kline>>(*response);
}
}  // namespace stonks::binance