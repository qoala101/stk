#include "binance_client.h"

#include "binance_settings.h"

namespace stonks::binance {
BinanceClient::BinanceClient() : client_{settings::GetBaseRestUri()} {}

[[nodiscard]] auto BinanceClient::GetExchangeInfo() -> ExchangeInfo {
  return std::any_cast<ExchangeInfo>(client_.Execute(
      network::EndpointDesc{.method = web::http::methods::GET,
                            .relative_uri = "/api/v3/exchangeInfo",
                            .response_body = json::Type<ExchangeInfo>{}}));
}
}  // namespace stonks::binance