#include "binance_api.h"

#include <spdlog/spdlog.h>

#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_settings.h"
#include "enum_conversions.h"
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

  auto symbols_range = symbols_array |
                       ranges::views::filter(has_usdt_quote_asset) |
                       ranges::views::filter(has_symbol_field) |
                       ranges::views::transform(get_symbol_field);

  return std::vector<stonks::binance::Symbol>{symbols_range.begin(),
                                              symbols_range.end()};
}
}  // namespace

namespace stonks::binance {
std::optional<std::vector<Symbol>> GetSymbols() {
  const auto exchange_info =
      rest::RestRequest{web::http::methods::GET,
                        "https://testnet.binance.vision/api/v3/exchangeInfo"}
          .SendAndGetResponse();

  if (!exchange_info.has_value()) {
    spdlog::error("Cannot get symbols");
    return std::nullopt;
  }

  return ParseSymbolsFromExchangeInfo(*exchange_info);
}

std::optional<Balances> GetBalances() {
  auto request =
      rest::RestRequest{web::http::methods::GET,
                        "https://testnet.binance.vision/api/v3/account"}
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("timestamp", utils::GetUnixTimeMillisAsString());
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

std::optional<PlaceOrderResult> PlaceOrder(const Symbol &symbol, Side side,
                                           Type type,
                                           std::optional<double> quantity,
                                           std::optional<double> price,
                                           TimeInForce time_in_force) {
  auto request =
      rest::RestRequest{web::http::methods::POST,
                        "https://testnet.binance.vision/api/v3/order"}
          .AddHeader("X-MBX-APIKEY", settings::GetApiKey())
          .AddParameter("symbol", symbol)
          .AddParameter("side", magic_enum::enum_name(side))
          .AddParameter("type", magic_enum::enum_name(type))
          .AddParameter("timeInForce", magic_enum::enum_name(time_in_force))
          .AddParameter("timestamp", utils::GetUnixTimeMillisAsString());

  if (quantity.has_value()) {
    request.AddParameter("quantity", std::to_string(*quantity));
  }

  if (price.has_value()) {
    request.AddParameter("price", std::to_string(*price));
  }

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
}  // namespace stonks::binance