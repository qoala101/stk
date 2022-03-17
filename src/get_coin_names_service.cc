#include "get_coin_names_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

namespace
{
  std::optional<web::json::value> CoinNamesFromExchangeInfo(const web::json::value &exchange_info)
  {
    if (!exchange_info.has_field("symbols"))
    {
      return std::nullopt;
    }

    const auto &symbols_value = exchange_info.at("symbols");

    if (!symbols_value.is_array())
    {
      return std::nullopt;
    }

    const auto &symbols_array = symbols_value.as_array();
    const auto has_symbol_field = [](const web::json::value &value)
    {
      return value.has_string_field("symbol");
    };
    const auto get_symbol_field = [](const web::json::value &value)
    {
      return value.at("symbol").as_string();
    };

    auto coin_names_range = symbols_array | (std::views::filter(has_symbol_field) | std::views::transform(get_symbol_field));
    const auto coin_names = std::vector<web::json::value>(coin_names_range.begin(), coin_names_range.end());
    return web::json::value::array(coin_names);
  }

  void HandleGetRequest(const web::http::http_request &request)
  {
    spdlog::info(BOOST_CURRENT_FUNCTION);

    spdlog::info("Requesting exchange info...");
    auto http_client = web::http::client::http_client{"https://api.binance.com/api/v3/exchangeInfo"};
    const auto response = http_client.request(web::http::methods::GET).get();
    const auto exchange_info = response.extract_json().get();
    spdlog::info("Got exchange info response");

    const auto coin_names = CoinNamesFromExchangeInfo(exchange_info);

    if (!coin_names.has_value())
    {
      spdlog::info("Exchange info is broken");
      request.reply(web::http::status_codes::NotFound);
      return;
    }

    request.reply(web::http::status_codes::OK, coin_names.value());
    spdlog::info("Extracted coin names");
  }
}

namespace stonks
{
  pplx::task<void> GetCoinNamesService::Start()
  {
    http_listener_ = web::http::experimental::listener::http_listener{"http://localhost:6506/api/coin_names"};
    http_listener_.support(web::http::methods::GET, HandleGetRequest);
    return http_listener_.open();
  }

  pplx::task<void> GetCoinNamesService::Stop()
  {
    return http_listener_.close();
  }
}