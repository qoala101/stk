#include "endpoints_binance.h"

#include <cpprest/http_msg.h>

#include <optional>
#include <string>

#include "binance_types.h"



namespace stonks {
auto endpoints::Binance::GetExchangeInfo() -> network::EndpointDesc {
  return {.method = web::http::methods::GET,
          .relative_uri = "/api/v3/exchangeInfo"};
}
}  // namespace stonks