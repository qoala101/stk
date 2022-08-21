#include "binance_api.h"

#include <bits/exception.h>
#include <cpprest/details/basic_types.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <utility>

#include "binance_settings.h"
#include "json_conversions.h"
#include "rest_request.h"
#include "utils.h"

namespace stonks::binance {
namespace {
std::optional<std::vector<std::string>> ParseSymbolsFromExchangeInfo(
    const web::json::value &exchange_info) {
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

std::optional<ExchangeInfo> GetExchangeInfo() {
  const auto response =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/exchangeInfo")
          .SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get exchange info");
    return std::nullopt;
  }

  return json::ParseFromJson<ExchangeInfo>(*response);
}

std::optional<std::string> GetBalances() {
  auto request =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
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

std::variant<std::monostate, PlaceOrderAcknowledgement, ApiError> PlaceOrder(
    std::string_view symbol, OrderSide side, OrderType type,
    std::optional<OrderTimeInForce> time_in_force,
    std::optional<double> quantity, std::optional<double> quote_order_quantity,
    std::optional<double> price,
    std::optional<std::string_view> new_client_order_id,
    std::optional<double> stop_price, std::optional<int64_t> trailing_delta,
    std::optional<double> iceberg_quantity,
    std::optional<NewOrderResponseType> new_order_response_type,
    std::optional<int64_t> receiving_window,
    std::chrono::milliseconds timestamp) {
  auto request =
      network::RestRequest{web::http::methods::POST, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/order")
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("symbol", symbol)
          .AddParameter("side", side)
          .AddParameter("type", type)
          .AddParameter("timeInForce", time_in_force)
          .AddParameter("quantity", quantity)
          .AddParameter("quoteOrderQty", quote_order_quantity)
          .AddParameter("price", price)
          .AddParameter("newClientOrderId", new_client_order_id)
          .AddParameter("stopPrice", stop_price)
          .AddParameter("trailingDelta", trailing_delta)
          .AddParameter("icebergQty", iceberg_quantity)
          .AddParameter("newOrderRespType", new_order_response_type)
          .AddParameter("recvWindow", receiving_window)
          .AddParameter("timestamp", timestamp);

  const auto params = request.GetParametersAsString();
  const auto signed_params =
      utils::SignUsingHmacSha256(params, settings::GetSecretKey());
  const auto response =
      request.AddParameter("signature", signed_params).SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot place order");
    return std::monostate{};
  }

  const auto acknowledgement =
      json::ParseFromJsonNoThrow<PlaceOrderAcknowledgement>(*response);

  if (acknowledgement.has_value()) {
    return *acknowledgement;
  }

  const auto api_error = json::ParseFromJsonNoThrow<ApiError>(*response);

  if (api_error.has_value()) {
    return *api_error;
  }

  return std::monostate{};
}

std::optional<std::vector<OrderInfo>> GetAllOrders(
    std::string_view symbol, std::optional<int64_t> order_id,
    std::optional<std::chrono::milliseconds> start_time,
    std::optional<std::chrono::milliseconds> end_time, std::optional<int> limit,
    std::optional<int64_t> receiving_window,
    std::chrono::milliseconds timestamp) {
  auto request =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/allOrders")
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("symbol", symbol)
          .AddParameter("orderId", order_id)
          .AddParameter("startTime", start_time)
          .AddParameter("endTime", end_time)
          .AddParameter("limit", limit)
          .AddParameter("recvWindow", receiving_window)
          .AddParameter("timestamp", timestamp);

  const auto params = request.GetParametersAsString();
  const auto signed_params =
      utils::SignUsingHmacSha256(params, settings::GetSecretKey());
  const auto response =
      request.AddParameter("signature", signed_params).SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get all orders");
    return std::nullopt;
  }

  return json::ParseFromJson<std::vector<OrderInfo>>(*response);
}

std::optional<OrderInfo> QueryOrder(
    std::string_view symbol, std::optional<int64_t> order_id,
    std::optional<std::string> original_client_order_id,
    std::optional<int64_t> receiving_window,
    std::chrono::milliseconds timestamp) {
  auto request =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/order")
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("symbol", symbol)
          .AddParameter("orderId", order_id)
          .AddParameter("origClientOrderId", original_client_order_id)
          .AddParameter("recvWindow", receiving_window)
          .AddParameter("timestamp", timestamp);

  const auto params = request.GetParametersAsString();
  const auto signed_params =
      utils::SignUsingHmacSha256(params, settings::GetSecretKey());
  const auto response =
      request.AddParameter("signature", signed_params).SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot query order");
    return std::nullopt;
  }

  return json::ParseFromJson<OrderInfo>(*response);
}

std::optional<std::vector<Kline>> GetKlines(
    std::string_view symbol, CandleInterval interval,
    std::optional<std::chrono::milliseconds> start_time,
    std::optional<std::chrono::milliseconds> end_time,
    std::optional<int> limit) {
  const auto response =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
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

  return json::ParseFromJson<std::vector<Kline>>(*response);
}

std::optional<AverageSymbolPrice> GetAverageSymbolPrice(
    std::string_view symbol) {
  const auto response =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/avgPrice")
          .AddParameter("symbol", symbol)
          .SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get average symbol price");
    return std::nullopt;
  }

  return json::ParseFromJson<AverageSymbolPrice>(*response);
}

std::optional<SymbolPrice> GetSymbolPrice(
    std::optional<std::string_view> symbol) {
  const auto response =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/ticker/price")
          .AddParameter("symbol", symbol)
          .SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get symbol price");
    return std::nullopt;
  }

  return json::ParseFromJson<SymbolPrice>(*response);
}

std::optional<std::vector<SymbolPrice>> GetAllSymbolsPrices() {
  const auto response =
      network::RestRequest{web::http::methods::GET, settings::GetBaseRestUri()}
          .AppendUri("/api/v3/ticker/price")
          .SendAndGetResponse();

  if (!response.has_value()) {
    spdlog::error("Cannot get all symbols prices");
    return std::nullopt;
  }

  return json::ParseFromJson<std::vector<SymbolPrice>>(*response);
}
}  // namespace stonks::binance