#include "get_symbols_service.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

#include <range/v3/to_container.hpp>
#include <range/v3/view/transform.hpp>

#include "binance_api.h"

namespace stonks {
namespace {
void HandleGetRequest(const web::http::http_request &request) {
  spdlog::info("Got {} request on {}", request.method(),
               request.request_uri().to_string());
  const auto exchange_info = binance::GetExchangeInfo();

  if (!exchange_info.has_value()) {
    request.reply(web::http::status_codes::NotFound);
    spdlog::info("Sent NotFound response");
    return;
  }

  auto json_symbols = exchange_info->symbols |
                      ranges::views::transform(
                          [](const binance::SymbolExchangeInfo &symbol_info) {
                            return web::json::value{symbol_info.base_asset +
                                                    symbol_info.quote_asset};
                          }) |
                      ranges::to_vector;
  const auto symbols_array = web::json::value::array(std::move(json_symbols));
  request.reply(web::http::status_codes::OK, symbols_array);
  spdlog::info("Sent OK response");
}
}  // namespace

pplx::task<void> GetSymbolsService::Start() {
  http_listener_ = web::http::experimental::listener::http_listener{
      "http://localhost:6506/api/symbols"};
  http_listener_.support(web::http::methods::GET, HandleGetRequest);
  return http_listener_.open();
}

pplx::task<void> GetSymbolsService::Stop() { return http_listener_.close(); }
}  // namespace stonks