#include "endpoints_binance.h"

namespace stonks {
auto BinanceEndpoints::GetExchangeInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/api/v3/exchangeInfo",
          .response_body = json::Type<binance::ExchangeInfo>{}};
}
}  // namespace stonks