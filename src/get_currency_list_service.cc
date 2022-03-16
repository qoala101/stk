#include "get_currency_list_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

namespace
{
  void HandleGetRequest(const web::http::http_request &request)
  {
    spdlog::info(BOOST_CURRENT_FUNCTION);

    spdlog::info("Getting currency list...");
    auto http_client = web::http::client::http_client{"https://api.binance.com/api/v3/exchangeInfo"};
    auto request_task = http_client.request(web::http::methods::GET);
    auto response = request_task.get();
    request.reply(web::http::status_codes::OK, response.extract_json().get());
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