#include "get_currency_list_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

namespace
{
  std::optional<web::json::value> ExchangeInfoToCurrencyList(const web::json::value &exchange_info)
  {
    if (!exchange_info.has_field("symbols"))
    {
      return std::nullopt;
    }

    const auto &currency_info_value = exchange_info.at("symbols");

    if (!currency_info_value.is_array())
    {
      return std::nullopt;
    }

    const auto &currency_info_array = currency_info_value.as_array();
    const auto has_symbol_field = [](const web::json::value &value)
    {
      return value.has_string_field("symbol");
    };
    const auto get_symbol_field = [](const web::json::value &value)
    {
      return value.at("symbol").as_string();
    };

    auto currencies_range = currency_info_array | (std::views::filter(has_symbol_field) | std::views::transform(get_symbol_field));
    const auto currency_names = std::vector<web::json::value>(currencies_range.begin(), currencies_range.end());
    return web::json::value::array(currency_names);
  }

  void HandleGetRequest(const web::http::http_request &request)
  {
    spdlog::info(BOOST_CURRENT_FUNCTION);

    spdlog::info("Requesting exchange info...");
    auto http_client = web::http::client::http_client{"https://api.binance.com/api/v3/exchangeInfo"};
    const auto response = http_client.request(web::http::methods::GET).get();
    const auto exchange_info = response.extract_json().get();
    spdlog::info("Got exchange info response");

    const auto currency_list = ExchangeInfoToCurrencyList(exchange_info);

    if (!currency_list.has_value())
    {
      spdlog::info("Exchange info is broken");
      request.reply(web::http::status_codes::NotFound);
      return;
    }

    request.reply(web::http::status_codes::OK, currency_list.value());
    spdlog::info("Extracted currency list");
  }
}

namespace stonks
{
  pplx::task<void> GetCurrencyListService::Start()
  {
    http_listener_ = web::http::experimental::listener::http_listener{"http://localhost:6506/api/currency_list"};
    http_listener_.support(web::http::methods::GET, HandleGetRequest);
    return http_listener_.open();
  }

  pplx::task<void> GetCurrencyListService::Stop()
  {
    return http_listener_.close();
  }
}